import pandas as pd
from sklearn.model_selection import train_test_split
from pathlib import Path
import spacy
from spacy.tokens import DocBin
from spacy.training import Example


def create_spacy_docs(nlp, texts, labels):
    doc_bin = DocBin()
    for text, label in zip(texts, labels):
        doc = nlp.make_doc(text)
        cats = {cat: cat == label for cat in label_set}
        example = Example.from_dict(doc, {"cats": cats})
        doc_bin.add(example.reference)
    return doc_bin


def main():
    df = pd.read_csv("./dataset/news_categories.csv")
    df["text"] = df["headline"].fillna("") + " " + df["short_description"].fillna("")
    df["label"] = df["category"].astype("category")

    global label_set
    label_set = sorted(df["label"].unique().tolist())

    train_df, dev_df = train_test_split(
        df, stratify=df["label"], test_size=0.2, random_state=42
    )

    nlp = spacy.blank("en")

    Path("corpus").mkdir(exist_ok=True)

    create_spacy_docs(nlp, train_df["text"], train_df["label"]).to_disk(
        "corpus/train.spacy"
    )
    create_spacy_docs(nlp, dev_df["text"], dev_df["label"]).to_disk("corpus/dev.spacy")

    print(f"Labels: {label_set}")
    with open("labels.txt", "w") as f:
        f.write("\n".join(label_set))


if __name__ == "__main__":
    main()
