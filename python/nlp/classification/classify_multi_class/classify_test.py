from transformers import pipeline, AutoModelForSequenceClassification, AutoTokenizer
import time


def main():
    print("Loading model components...")
    try:
        model = AutoModelForSequenceClassification.from_pretrained(
            "./model/trained_classifier"
        )
        tokenizer = AutoTokenizer.from_pretrained("./model/trained_classifier")
        print("Model components loaded successfully!")
    except FileNotFoundError:
        print(
            "Error: Model files not found. Please ensure model and tokenizer files are in the './model/' directory."
        )
        return

    clf = pipeline("text-classification", model=model, tokenizer=tokenizer)

    print("\nPlease paste the text you want to categorize and press Enter:")
    user_input_text = input()

    print("\nProcessing...")
    start_time = time.time()

    predicted_result = clf(user_input_text)

    end_time = time.time()
    processing_time = end_time - start_time

    predicted_first_result = predicted_result[0]

    print("\n---")
    print(f"Predicted Category: {predicted_first_result['label']}")
    print(f"Score: {predicted_first_result['score']:.6f}")
    print(f"Processing Time: {processing_time:.2f} seconds")
    print("---")


if __name__ == "__main__":
    main()
