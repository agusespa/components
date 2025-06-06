import spacy
import time


def load_labels():
    with open("labels.txt", "r") as f:
        return f.read().splitlines()


def main():
    nlp = spacy.load("./model/model-best")
    labels = load_labels()

    print("\nEnter your news text:")
    text = input()

    start = time.time()
    doc = nlp(text)
    elapsed = time.time() - start

    scores = {label: doc.cats.get(label, 0.0) for label in labels}
    sorted_scores = sorted(scores.items(), key=lambda x: x[1], reverse=True)
    best_label, best_score = sorted_scores[0]

    print("\n---")
    print(f"Predicted Category: {best_label}")
    print(f"Score: {best_score:.4f}")
    print(f"Processing Time: {elapsed:.2f} seconds")
    print("---")


if __name__ == "__main__":
    main()
