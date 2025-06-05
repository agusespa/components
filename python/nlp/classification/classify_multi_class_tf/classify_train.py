import pandas as pd
import nltk
from nltk.tokenize import word_tokenize
from nltk.corpus import stopwords
from nltk.stem import PorterStemmer
from sklearn.model_selection import train_test_split
from sklearn.feature_extraction.text import TfidfVectorizer
import joblib
from sklearn.svm import LinearSVC
from sklearn.metrics import accuracy_score
from sklearn.calibration import CalibratedClassifierCV


def preprocess_text(text):
    stop_words = set(stopwords.words("english"))
    stemmer = PorterStemmer()

    text = text.lower()
    tokens = word_tokenize(text)
    tokens = [token for token in tokens if token.isalpha()]
    tokens = [token for token in tokens if token not in stop_words]
    tokens = [stemmer.stem(token) for token in tokens]
    return " ".join(tokens)


def main():
    nltk.download("punkt")
    nltk.download("stopwords")


    print("Preparing the dataset...")

    df = pd.read_csv("./dataset/news_categories.csv")

    df["text"] = df["headline"].fillna("") + " " + df["short_description"].fillna("")

    df["label"] = df["category"].astype("category")
    df["label_id"] = df["label"].cat.codes

    df["processed_text"] = df["text"].apply(preprocess_text)

    X_train, X_test, y_train, y_test = train_test_split(
        df["processed_text"], df["label_id"], test_size=0.2, stratify=df["label_id"]
    )

    print("Vectorizing...")

    vectorizer = TfidfVectorizer(
        ngram_range=(1, 2),
    )

    X_train_vectorized = vectorizer.fit_transform(X_train)
    X_test_vectorized = vectorizer.transform(X_test)


    print("Model training...")

    base_model = LinearSVC(C=10, loss="squared_hinge")
    base_model.fit(X_train_vectorized, y_train)

    calibrated_model = CalibratedClassifierCV(base_model, method="isotonic", cv=5)
    calibrated_model.fit(X_train_vectorized, y_train)

    y_pred = calibrated_model.predict(X_test_vectorized)
    print("Accuracy (of calibrated model):", accuracy_score(y_test, y_pred))

    label_map = dict(enumerate(df["label"].cat.categories))

    joblib.dump(calibrated_model, "./model/calibrated_linear_svc_model.pkl")
    joblib.dump(vectorizer, "./model/tfidf_vectorizer.pkl")
    joblib.dump(label_map, "./model/label_map.pkl")

    print("Calibrated model and label map saved.")

if __name__ == "__main__":
    main()
