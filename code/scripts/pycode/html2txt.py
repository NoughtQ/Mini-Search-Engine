'''
In this script, 
we will extract all the text content from the HTML files in a folder and write it to a new text file.  
We will also write the title of each HTML file to a separate text file.(It is used in "getStopWord.cpp")
PS: the html files are downloaded from https://github.com/TheMITTech/shakespeare

If you want to run this scripts,
you need to install BeautifulSoup4 first.
You can install by : pip install beautifulsoup4
'''

# *** Notice: I suggest you to run this script in the main directory of the project. ***

import os
from bs4 import BeautifulSoup   # solve the html parsing problem

# the following files are not useful for our project, so we will skip them.
black_list = ['test.html','full.html','index.html','works.html','news.html','google30de5406cd6ff416.html','googlefea2bbd41b5b400b.html']
title = set()

def extract_all_text_to_txt(html_path, txt_path):
    with open(html_path, 'r', encoding='utf-8') as file:
        soup = BeautifulSoup(file, 'lxml')
        # Remove the content under the <head> tag
        for head in soup.find_all('head'):
            head.decompose()
        # Remove the content under the <table> tag
        for table in soup.find_all('table'):
            table.decompose()
        # get the text content and seperate it by '\n'
        text_content = soup.get_text(separator='\n',strip=True)
    # write the text content to a new file
    with open(txt_path, 'w', encoding='utf-8') as txt_file:
        txt_file.write(text_content)

def extract_text_from_folder(folder_path,txt_path):
    # iterate through all the files in the folder
    for root, dirs, files in os.walk(folder_path):
        for file in files:
            if file.endswith('.html'):
                if file in black_list:
                    continue
                # get the full path of the html file
                html_path = os.path.join(root, file)
                # remove the '.html' extension from the file name to get the title
                title.add(file[:-5])
                # get the name of the txt file
                txt_filename = file[:-5] + '.txt' 
                # write path is the path of the txt_title.txt
                write_path = os.path.join(txt_path, txt_filename)
                print(f"Extracting text from {html_path} and writing to {txt_path}:")
                extract_all_text_to_txt(html_path, write_path)
                print(f"Text written to {write_path}\n")

if __name__ == '__main__':
    write_path = 'code/source/shakespeare_works'
    os.makedirs(write_path, exist_ok=True)
    folder_path = 'code/source/shakespeare-master'
    extract_text_from_folder(folder_path,write_path)
    # write the title to a new file
    with open('code/source/txt_title.txt', 'w', encoding='utf-8') as title_file:
        for t in title:
            title_file.write(t+'\n')
