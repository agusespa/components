import joblib
import time
from classify_train import preprocess_text


def predict_category(text, model, vectorizer, label_map):
    processed = preprocess_text(text)
    vectorized = vectorizer.transform([processed])

    probabilities = model.predict_proba(vectorized)[0]

    label_id = probabilities.argmax()

    predicted_label = label_map[label_id]

    score = probabilities[label_id]

    return {"label": predicted_label, "score": float(f"{score:.6f}")}


def main():
    print("Loading model components...")
    try:
        model = joblib.load("./model/calibrated_linear_svc_model.pkl")
        vectorizer = joblib.load("./model/tfidf_vectorizer.pkl")
        label_map = joblib.load("./model/label_map.pkl")
        print("Model components loaded successfully!")
    except FileNotFoundError:
        print(
            "Error: Model files not found. Please ensure 'calibrated_linear_svc_model.pkl', 'tfidf_vectorizer.pkl', and 'label_map.pkl' are in the './model/' directory."
        )
        return

    print("\nPlease paste the news article you want to categorize and press Enter:")
    user_input_text = input()

    start_time = time.time()

    predicted_result = predict_category(user_input_text, model, vectorizer, label_map)

    end_time = time.time()
    processing_time = end_time - start_time

    print("\n---")
    print(f"Predicted Category: {predicted_result['label']}")
    print(f"Score: {predicted_result['score']:.6f}")
    print(f"Processing Time: {processing_time:.2f} seconds")
    print("---")


if __name__ == "__main__":
    main()
