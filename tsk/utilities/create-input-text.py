# If you want this as a file, just pipe it
# Sample: python create-input-text.py > IN.tsk

inputList = [
    # Node input goes here
    2,
    0,
    0,
    3,
    4,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    3,
    3,
    0,
    0,
    3,
    0,
    3,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    0,
    0,
    2,
    4,
    1,
    3,
]

for input in inputList:
    print(f"MOV {input} DOWN")
