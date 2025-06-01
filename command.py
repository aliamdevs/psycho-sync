#!/usr/bin/env python3
import argparse
import requests
import os
import base64
from typing import Tuple

def encode_file_to_base64(file_path: str) -> str:
    try:
        with open(file_path, 'rb') as f:
            file_content = f.read()
            print( base64.b64encode(file_content).decode('utf-8'))
            return base64.b64encode(file_content).decode('utf-8')
    except Exception as e:
        raise ValueError(f"Error reading file: {str(e)}")

def save_base64_to_file(base64_data: str, file_type: str, output_path: str):
    binary_data = base64.b64decode(base64_data)
    if not output_path.endswith(file_type):
        output_path += '.'+ file_type
    with open(output_path, 'wb') as f:
        f.write(binary_data)
    
    return output_path

def send_to_api(mode: str, name: str, content: str, file_type: str) -> requests.Response:
    inx = '0'
    if(mode == "upload"):
        inx = '1'
    elif(mode == "download"):
        inx = '0'
    elif(mode == "tree"):
        inx = '10'
    elif(mode == "format"):
        inx = '11'
    elif(mode == "storage"):
        inx = '100'

    if(inx == '0' or inx == '1'):
        data = {
        'mode': inx,
        'context': content,
        'name': name + '~' + file_type,
        }
    else:
        data ={
            'mode': inx,
            'context': content,
            'name': name,
        }
    # print(data)
    try:
        response = requests.post('http://3.3.0.0/3430', data=data)
        return response
    except requests.exceptions.RequestException as e:
        raise ValueError(f"API Error: {str(e)}")

def main():
    parser = argparse.ArgumentParser(description='Send And Get File From Local Server')
    parser.add_argument('-m', '--mode', choices=['upload','download','tree','storage','format','help'], 
                      required=True, help='Operation mode')
    parser.add_argument('-n', '--name', help='name/attribute')
    parser.add_argument('-p', '--path', help='file path/context')
    parser.add_argument('-t', '--type', choices=["txt" , "png" , "mp3" ], help='file type')
    
    args = parser.parse_args()
    
    try:
        if args.mode == "upload":
            print(" --- UPLOAD MODE ---")
            if(args.type == 'txt'):
                try:
                    with open(args.path, 'rb') as f:
                        file_content = f.read()
                except Exception as e:
                    raise ValueError(f"Error reading file: {str(e)}")
                print(file_content)
                response = send_to_api(args.mode, args.name , file_content, args.type)
                if(response.json()['code'] == "200"):
                    print(f" >> Done - Name : "+args.name +"\n")
            else:    
                if not args.path :
                    if not args.name :
                        if not args.type :  
                            raise ValueError("File Path & Name & Type are required for Upload Mode")
                if not os.path.isfile(args.path):
                    raise NameError("File does not exist")
                
                if(args.path == "*"):
                    args.path = "C:\\Users\\Sorena\\Desktop\\DOWNLOAD\\"+args.name+'.'+args.type

                encoded_content = encode_file_to_base64(args.path)
                response = send_to_api(args.mode, args.name , encoded_content, args.type)
                if(response.json()['code'] == "200"):
                    print(f" >> Done - Name : "+args.name +"\n")

        elif args.mode == "help":
            print("\n>> Help mode -> Available commands:")
            print("   (upload) - Upload a file")
            print("   (download) - Download a file")
            print("   (dir) - List directory")
            print("   (storage) - Check storage")
            print("   (format) - Format options")
            print("   (help) - Show this help\n")
            
        elif args.mode == "download":
            print(" --- DOWNLOAD MODE ---")
            response = send_to_api(args.mode, args.name , "0" , args.type)
            if(args.type == 'txt'):
                with open('C:\\Users\\Sorena\\Desktop\\DOWNLOAD\\'+args.name+'.png', 'wb') as f:
                    f.write(response.json()['data'])
            else:
                save_base64_to_file(response.json()['data'], args.type, 'C:\\Users\\Sorena\\Desktop\\DOWNLOAD\\'+args.name)
            print(f" >> Done - Saved To : C:\\Users\\Sorena\\Desktop\\DOWNLOAD\\")

        elif args.mode == "tree":
            print(" --- Tree ---")
            headers = {
                'Content-Type': 'application/x-www-form-urlencoded',
            }
            data = 'name=/&context=0&mode=10'
            response = requests.post('http://44.0.0.4/api', headers=headers, data=data)
            TREE = response.text
            # TREE = TREE.split('Listing directory:')[1]
            # TREE = TREE.split('"}')[0]
            print(TREE)
            
        elif args.mode == "storage":
            response = send_to_api(args.mode, "0", "0" , '')
            print(response.text)
            
        elif args.mode == "format":
            response = send_to_api(args.mode, "/", "0" , '')
            print(" >> Card Is Earased .")
            
    except ValueError as e:
        print(f"Error: {str(e)}")
        exit(1)
    except NameError as e:
        print(f"Error: {str(e)}")
        exit(1)

if __name__ == "__main__":
    main()