from cs50 import get_string


def main():
    text = get_string("Text: ")
    letters = countLetters(text)
    words = countWords(text)
    sentences = countSentences(text)
    level = readingLevel(letters, words, sentences)
    if level < 1:
        print("Before Grade 1")
    elif level >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {level}")


def countLetters(text):
    letterCount = 0

    for element in text:
        if element >= 'a' and element <= 'z':
            letterCount += 1
        elif element >= 'A' and element <= 'Z':
            letterCount += 1
    return letterCount


def countWords(text):
    wordCount = 1

    for element in text:
        if element == ' ':
            wordCount += 1
    return wordCount


def countSentences(text):
    sentenceCount = 0

    for element in text:
        if element == '.' or element == '!' or element == '?':
            sentenceCount += 1
    return sentenceCount


def readingLevel(letters, words, sentences):
    L = (letters / words) * 100
    S = (sentences / words) * 100
    gradeLevel = round((0.0588 * L) - (0.296 * S) - 15.8)
    return gradeLevel

main()
