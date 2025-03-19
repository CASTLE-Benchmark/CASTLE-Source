import math
import re
import clang.cindex


def _clang_deconstruct_expressions(code):
    index = clang.cindex.Index.create()
    pattern = r'\b([a-zA-Z_][a-zA-Z0-9_]*)\s*\([^)]*\)\s*'
    file_name = 'main.c'

    # Parse the string as a translation unit
    tu = index.parse(
        path=file_name,
        unsaved_files=[(file_name, code)],  # Provide the code as an unsaved file
        options=0
    )
    # Find all matches
    matches = re.findall(pattern, code)

    operators = []
    operands = []
    root_node = tu.cursor
    for token in root_node.get_tokens():
        if token.kind == clang.cindex.TokenKind.PUNCTUATION:
            operators.append(token.spelling)
        elif token.kind == clang.cindex.TokenKind.LITERAL:
            operands.append(token.spelling)
        elif token.kind == clang.cindex.TokenKind.KEYWORD:
            if token.cursor.kind == clang.cindex.CursorKind.VAR_DECL or token.cursor.kind.is_statement():
                operators.append(token.spelling)
            else:
                operands.append(token.spelling)
        elif token.kind == clang.cindex.TokenKind.IDENTIFIER:
            if token.cursor.kind == clang.cindex.CursorKind.FUNCTION_DECL:
                operators.append(token.spelling)
            elif token.spelling in matches:
                operators.append(token.spelling)
            else:
                operands.append(token.spelling)
    operand_map = {}
    operator_map = {}
    for item in operands:
        operand_map[item] = operand_map.get(item, 0) + 1
    for item in operators:
        if item == "}" or item == "{":
            operator_map["{}"] = operator_map.get("{}", 0) + 1
        elif item == ")" or item == "(":
            operator_map["()"] = operator_map.get("()", 0) + 1 
        else:
            operator_map[item] = operator_map.get(item, 0) + 1
    operator_map["{}"] = operator_map.get("{}", 0) // 2
    operator_map["()"] = operator_map.get("()", 0) // 2
    return operator_map, operand_map


def halstead_volume(content):
    # Formula: https://en.wikipedia.org/wiki/Halstead_complexity_measures
    operators, operands = _clang_deconstruct_expressions(content)

    n1 = len(operators)          # Number of distinct operators
    n2 = len(operands)           # Number of distinct operands
    N1 = sum(operators.values()) # Total number of operators
    N2 = sum(operands.values())  # Total number of operands

    # Halstead volume calculation
    vocabulary = n1 + n2
    length = N1 + N2
    if vocabulary == 0:
        return 0
    else:
        return length * math.log2(vocabulary)


def maintainability_index(cyclomatic_complexity, nloc, halstead_volume):
    # Formula: https://learn.microsoft.com/en-us/visualstudio/code-quality/code-metrics-maintainability-index-range-and-meaning?view=vs-2022
    if nloc > 0 and halstead_volume > 0:
        return max(0.0, min(171 - 5.2 * math.log(halstead_volume) - 0.23 * cyclomatic_complexity - 16.2 * math.log(nloc), 100.0))
    else:
        return 100.0 # For extremely simple code
    
if __name__ == '__main__':
    with open('dataset/22/CASTLE-22-1.c', 'r') as f:
        content = f.read()
        print(halstead_volume(content))
