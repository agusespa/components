import pandas as pd
import fasttext
import nltk
from nltk.tokenize import word_tokenize
from sklearn.model_selection import train_test_split
from nltk.corpus import stopwords
from nltk.stem import PorterStemmer


def save_fasttext_format(df, filename):
    with open(filename, "w", encoding="utf-8") as f:
        for tokens, label in zip(df["processed_text"], df["label_id"]):
            text = " ".join(tokens)
            f.write(f"__label__{label} {text}\n")


def preprocess(text):
    stop_words = set(stopwords.words("english"))
    stemmer = PorterStemmer()

    text = text.lower()
    tokens = word_tokenize(text)
    tokens = [token for token in tokens if token.isalpha()]
    tokens = [token for token in tokens if token not in stop_words]
    tokens = [stemmer.stem(token) for token in tokens]
    return tokens


def main():
    nltk.download("punkt")
    nltk.download("stopwords")


    print("Preparing the dataset...")

    df = pd.read_csv("./dataset/news_categories.csv")
    df["text"] = df["headline"].fillna("") + " " + df["short_description"].fillna("")
    df["label"] = df["category"].astype("category")
    df["label_id"] = df["label"].cat.codes
    df["processed_text"] = df["text"].apply(preprocess)

    train_df, test_df = train_test_split(
        df, stratify=df["label_id"], test_size=0.2, random_state=42
    )

    save_fasttext_format(train_df, "./dataset/fasttext_train.txt")
    save_fasttext_format(test_df, "./dataset/fasttext_test.txt")

    df[["label_id", "label"]].drop_duplicates().to_csv(
        "./model/label_map.csv", index=False
    )

    print("Training FastText classifier...")
    model = fasttext.train_supervised(
        input="./dataset/fasttext_train.txt",
        lr=0.5,
        epoch=50,
        wordNgrams=3,
        dim=300,
        minn=3,
        maxn=6,
        loss="ova",
        verbose=2,
    )

    result = model.test("./dataset/fasttext_test.txt")
    print(
        f"Test samples: {result[0]}, Precision: {result[1]:.4f}, Recall: {result[2]:.4f}"
    )

    model.save_model("./model/fasttext_model.bin")
    print("FastText model saved.")


if __name__ == "__main__":
    main()
