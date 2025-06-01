import base64
def prepareToSend(FILEPATH: str) -> str:
    try:
        with open(FILEPATH, 'rb') as f:
            file_content = f.read()
            coded_content = base64.b64encode(file_content).decode('utf-8')
            with open("/Users/ali/Desktop/inside.txt", 'wb') as f:
                f.write(file_content)
            with open("/Users/ali/Desktop/coded.txt", 'w') as f:
                f.write(coded_content)
    except Exception as e:
        raise ValueError(f"Error reading file: {str(e)}")