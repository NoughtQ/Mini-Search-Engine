import os
import re
import nltk
from nltk.stem import PorterStemmer
from nltk.tokenize import word_tokenize

nltk.download('punkt_tab')

# 定义一个函数来处理单个文件
def process_file(file_path,write_path):
    with open(file_path, 'r', encoding='utf-8') as f:
        text = f.read()
        text = re.sub(r'[^\w\s]', '', text)
        text = text.lower()
        words = word_tokenize(text)
        stemmer = PorterStemmer()
        stemmed_words = [stemmer.stem(word) for word in words]
        with open(write_path, 'w', encoding='utf-8') as f2:
            f2.write('\n'.join(stemmed_words))
            print(f'Processed {file_path} and wrote to {write_path}.')

    
source_path = 'shakespeare_works'
write = 'processed_shakespeare_works'
os.makedirs(write, exist_ok=True)
# 读取所有文件并进行预处理和提词干
for filename in os.listdir(source_path):
    if filename.endswith('.txt'):
        file_path = os.path.join(source_path, filename)
        write_path = os.path.join(write, filename)
        file_counter = process_file(file_path,write_path)
