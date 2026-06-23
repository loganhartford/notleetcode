def isValid(s: str) -> bool:
    stack = []
    brackets = {
        "[":"]",
        "{":"}",
        "(":")",
    }
    
    if len(s) % 2 != 0:
        return False
    
    for b in s:
        if b in brackets:
            stack.append(brackets[b])
        else:
            if len(stack) == 0 or stack.pop() != b:
                return False
    return True
