import spacy

class POSFeatureExtractor:
    def __init__(self):
        # Initialize the spaCy English language model
        self.nlp = spacy.load("en_core_web_sm")

    def pos_feature_vector(self, sentence):
        # Process the input sentence using spaCy
        doc = self.nlp(sentence)
        
        # Initialize a dictionary to store POS tag counts
        pos_counts = {}
        
        # Count POS tags in the sentence
        for token in doc:
            pos_tag = token.pos_
            if pos_tag in pos_counts:
                pos_counts[pos_tag] += 1
            else:
                pos_counts[pos_tag] = 1
        
        # Convert the POS tag counts into a feature vector
        feature_vector = [pos_counts.get(tag, 0) for tag in ["PRON", "NOUN", "VERB", "ADJ", "ADV"]]
        
        return feature_vector

# Example usage
if __name__ == "__main__":
    pos_extractor = POSFeatureExtractor()
    
    sentence1 = "The quick brown fox jumps over the lazy dog"
    feature_vector1 = pos_extractor.pos_feature_vector(sentence1)
    print("POS Feature Vector 1:", feature_vector1)
    
    sentence2 = "She sings beautifully"
    feature_vector2 = pos_extractor.pos_feature_vector(sentence2)
    print("POS Feature Vector 2:", feature_vector2)
