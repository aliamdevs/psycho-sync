import base64
def prepareToSave(DATA: str):
    try:
       with open("/Users/ali/Desktop/coded.txt", 'r') as f:
        file_content = f.read()
        binary_data = base64.b64decode(file_content)
        with open("/Users/ali/Desktop/mwithbase64.mp3", 'wb') as f:
            f.write(binary_data)
        with open("/Users/ali/Desktop/inside.txt", 'rb') as f:
            file_content = f.read()
            with open("/Users/ali/Desktop/mwithbin.mp3", 'wb') as f:
                f.write(file_content)
    except Exception as e:
        raise ValueError(f"Error reading file: {str(e)}")