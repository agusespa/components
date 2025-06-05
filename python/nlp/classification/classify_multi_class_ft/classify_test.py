import time
import fasttext
import pandas as pd


def load_label_map():
    df = pd.read_csv("./model/label_map.csv")
    id_to_label = dict(zip(df["label_id"], df["label"]))
    label_to_id = {v: k for k, v in id_to_label.items()}
    return id_to_label, label_to_id


def main():
    model = fasttext.load_model("./model/fasttext_model.bin")
    id_to_label, _ = load_label_map()

    print("\nEnter your news text:")
    user_input = input()

    start = time.time()
    predictions = model.predict(user_input, k=1)
    predicted_label = predictions[0][0].replace("__label__", "")
    elapsed = time.time() - start

    confidence = predictions[1][0]

    print("\n---")
    print(f"Predicted Category: {id_to_label[int(predicted_label)]}")
    print(f"Score: {confidence:.4f}")
    print(f"Processing Time: {elapsed:.2f} seconds")
    print("---")


if __name__ == "__main__":
    main()
