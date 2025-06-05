from datasets import Dataset
import evaluate
import pandas as pd
from transformers import (
    AutoModelForSequenceClassification,
    AutoTokenizer,
    DataCollatorWithPadding,
    Trainer,
    TrainingArguments,
)


def main():
    print("Preparing the dataset...")

    df = pd.read_csv("./dataset/news_categories.csv")

    df["text"] = df["headline"].fillna("") + " " + df["short_description"].fillna("")

    df["label"] = df["category"].astype("category")
    df["label_id"] = df["label"].cat.codes

    df_final = df[["text", "label_id"]].dropna()

    id2label = dict(enumerate(df["label"].cat.categories))
    label2id = {v: k for k, v in id2label.items()}

    dataset = Dataset.from_pandas(df_final)
    dataset = dataset.train_test_split(test_size=0.2, seed=42)

    checkpoint = "distilbert-base-uncased"
    tokenizer = AutoTokenizer.from_pretrained(checkpoint)

    def preprocess(example):
        model_input = tokenizer(example["text"], truncation=True)
        model_input["labels"] = example["label_id"]
        return model_input

    tokenized = dataset.map(preprocess, batched=True)
    tokenized = tokenized.remove_columns(["text", "label_id"])

    print("Preparing the model...")

    model = AutoModelForSequenceClassification.from_pretrained(
        checkpoint,
        num_labels=len(id2label),
        id2label=id2label,
        label2id=label2id,
    )

    training_args = TrainingArguments(
        output_dir="./results",
        eval_strategy="epoch",
        save_strategy="epoch",
        per_device_train_batch_size=16,
        per_device_eval_batch_size=16,
        num_train_epochs=3,
        weight_decay=0.01,
        load_best_model_at_end=True,
    )

    accuracy = evaluate.load("accuracy")

    def compute_metrics(eval_pred):
        logits, labels = eval_pred
        preds = logits.argmax(-1)
        return accuracy.compute(predictions=preds, references=labels)

    data_collator = DataCollatorWithPadding(tokenizer=tokenizer)

    trainer = Trainer(
        model=model,
        args=training_args,
        train_dataset=tokenized["train"],
        eval_dataset=tokenized["test"],
        data_collator=data_collator,
        compute_metrics=compute_metrics,
    )

    print("Training...")

    trainer.train()

    print("Evaluating...")

    metrics = trainer.evaluate()
    print(metrics)

    trainer.save_model("./model/trained_classifier")
    tokenizer.save_pretrained("./model/trained_classifier")


if __name__ == "__main__":
    main()
