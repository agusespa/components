import joblib
import time
import numpy as np
import gensim.downloader as api
from classify_train import preprocess

WORD_VECTORS_MODEL = None
EMBEDDING_DIM = None
IDF_DICT = None


def load_word2vec():
    global WORD_VECTORS_MODEL, EMBEDDING_DIM
    if WORD_VECTORS_MODEL is None:
        print("Loading Word2Vec model...")
        WORD_VECTORS_MODEL = api.load("word2vec-google-news-300")
        EMBEDDING_DIM = WORD_VECTORS_MODEL.vector_size
        print(f"Loaded Word2Vec with {EMBEDDING_DIM} dimensions.")


def load_idf_dict():
    global IDF_DICT
    tfidf = joblib.load("./model/tfidf_vectorizer.pkl")
    IDF_DICT = dict(zip(tfidf.get_feature_names_out(), tfidf.idf_))


def compute_document_vector(tokens):
    global WORD_VECTORS_MODEL, IDF_DICT, EMBEDDING_DIM
    vecs = [
        WORD_VECTORS_MODEL[word] * IDF_DICT.get(word, 1.0)
        for word in tokens
        if word in WORD_VECTORS_MODEL.key_to_index
    ]
    if vecs:
        return np.mean(vecs, axis=0).reshape(1, -1)
    else:
        return np.zeros((1, EMBEDDING_DIM))


def predict_category(text, model, label_map):
    tokens = preprocess(text)
    vector = compute_document_vector(tokens)
    probabilities = model.predict_proba(vector)[0]
    label_id = probabilities.argmax()
    return {
        "label": label_map[label_id],
        "score": float(f"{probabilities[label_id]:.6f}"),
    }


def main():
    global WORD_VECTORS_MODEL, EMBEDDING_DIM

    print("Loading model and metadata...")
    try:
        model = joblib.load("./model/mlp_word2vec_classifier.pkl")
        label_categories = joblib.load("./model/label_categories.pkl")
        label_map = dict(enumerate(label_categories))
        load_word2vec()
        load_idf_dict()
    except Exception as e:
        print(f"Failed to load model or metadata: {e}")
        return

    print("\nEnter your news text:")
    user_input = input()

    start = time.time()
    result = predict_category(user_input, model, label_map)
    elapsed = time.time() - start

    print("\n---")
    print(f"Predicted Category: {result['label']}")
    print(f"Confidence Score: {result['score']:.6f}")
    print(f"Processing Time: {elapsed:.2f} seconds")
    print("---")


if __name__ == "__main__":
    main()
