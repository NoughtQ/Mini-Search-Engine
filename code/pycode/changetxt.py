import os
from bs4 import BeautifulSoup

black_list = ['test.html','full.html','index.html','works.html','news.html','google30de5406cd6ff416.html','googlefea2bbd41b5b400b.html']

def extract_all_text_to_txt(html_path, txt_path):
    with open(html_path, 'r', encoding='utf-8') as file:
        soup = BeautifulSoup(file, 'lxml')
        # 移除 <head> 标签
        for head in soup.find_all('head'):
            head.decompose()
        # 移除 <table> 标签
        for table in soup.find_all('table'):
            table.decompose()
        # 获取所有文本内容并以换行符分隔
        text_content = soup.get_text(separator='\n',strip=True)
        
    with open(txt_path, 'w', encoding='utf-8') as txt_file:
        txt_file.write(text_content)

def extract_text_from_folder(folder_path,txt_path):
    with open('txt_title.txt', 'w', encoding='utf-8') as title_file:
        for root, dirs, files in os.walk(folder_path):
            for file in files:
                if file.endswith('.html'):
                    if file in black_list:
                        continue
                    html_path = os.path.join(root, file)
                    title_file.write(file[:-5] + '\n') 
                    txt_filename = file[:-5] + '.txt' 
                    write_path = os.path.join(txt_path, txt_filename)
                    print(f"Extracting text from {html_path} and writing to {txt_path}:")
                    extract_all_text_to_txt(html_path, write_path)
                    print(f"Text written to {write_path}\n")


os.makedirs('shakespeare_works', exist_ok=True)
folder_path = 'shakespeare-master'
extract_text_from_folder(folder_path,'shakespeare_works')