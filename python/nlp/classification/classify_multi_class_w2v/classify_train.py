import pandas as pd
import numpy as np
import gensim.downloader as api
import nltk
from nltk.tokenize import word_tokenize
from sklearn.model_selection import train_test_split
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.neural_network import MLPClassifier
from sklearn.metrics import accuracy_score
import joblib


# ---------------------------
# Preprocessing
# ---------------------------
def preprocess(text):
    tokens = word_tokenize(text.lower())
    return [token for token in tokens if token.isalpha()]


# ---------------------------
# TF-IDF-weighted Word2Vec
# ---------------------------
def get_weighted_word2vec_vectors(token_lists, word_vectors, idf_dict, vector_size):
    document_vectors = []
    for tokens in token_lists:
        vecs = [
            word_vectors[word] * idf_dict.get(word, 1.0)
            for word in tokens
            if word in word_vectors.key_to_index
        ]
        if vecs:
            document_vectors.append(np.mean(vecs, axis=0))
        else:
            document_vectors.append(np.zeros(vector_size))
    return np.array(document_vectors)


# ---------------------------
# Main Pipeline
# ---------------------------
def main():
    nltk.download("punkt")

    # Load dataset
    df = pd.read_csv("./dataset/news_categories.csv")
    df["text"] = df["headline"].fillna("") + " " + df["short_description"].fillna("")
    df["label"] = df["category"].astype("category")
    df["label_id"] = df["label"].cat.codes

    # Preprocess text
    df["processed_tokens"] = df["text"].apply(preprocess)

    X_tokens = df["processed_tokens"]
    y = df["label_id"]

    # Prepare corpus for TF-IDF (joined text)
    X_text_joined = X_tokens.apply(lambda tokens: " ".join(tokens))

    # Train-test split
    X_train_tokens, X_test_tokens, X_train_texts, X_test_texts, y_train, y_test = (
        train_test_split(
            X_tokens, X_text_joined, y, test_size=0.2, stratify=y, random_state=42
        )
    )

    # Fit TF-IDF on training data
    tfidf = TfidfVectorizer()
    tfidf.fit(X_train_texts)

    idf_dict = dict(zip(tfidf.get_feature_names_out(), tfidf.idf_))

    # Load pretrained Word2Vec
    print("Loading Word2Vec model...")
    word_vectors = api.load("word2vec-google-news-300")
    vector_size = word_vectors.vector_size

    # Compute document vectors
    print("Vectorizing documents with TF-IDF-weighted Word2Vec...")
    X_train_vec = get_weighted_word2vec_vectors(
        X_train_tokens, word_vectors, idf_dict, vector_size
    )
    X_test_vec = get_weighted_word2vec_vectors(
        X_test_tokens, word_vectors, idf_dict, vector_size
    )

    # Train classifier
    print("Training MLP classifier...")
    clf = MLPClassifier(hidden_layer_sizes=(100,), max_iter=300, random_state=42)
    clf.fit(X_train_vec, y_train)

    # Evaluate
    y_pred = clf.predict(X_test_vec)
    accuracy = accuracy_score(y_test, y_pred)
    print(f"Accuracy: {accuracy:.4f}")

    # Save model and metadata
    joblib.dump(clf, "./model/mlp_word2vec_classifier.pkl")
    joblib.dump(tfidf, "./model/tfidf_vectorizer.pkl")
    joblib.dump(df["label"].cat.categories, "./model/label_categories.pkl")
    print("Model and label mapping saved.")


if __name__ == "__main__":
    main()
