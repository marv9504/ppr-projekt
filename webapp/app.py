from flask import Flask, render_template, request
import subprocess
import os
import uuid

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def index():
    result_path = None
    original_path = None

    if request.method == 'POST':
        img = request.files['image']
        tol = request.form.get('tolerance', '30')

        uid = str(uuid.uuid4())
        ext = os.path.splitext(img.filename)[1].lower() or '.jpg'

        input_path = f'static/{uid}_input{ext}'
        output_path = f'static/{uid}_output.png'

        img.save(input_path)
        original_path = input_path

        # C-Programm ausführen
        subprocess.run(['../image_processor/lammbock.exe', input_path, output_path, tol])

        result_path = output_path

    return render_template('index.html', result=result_path, original=original_path)

if __name__ == '__main__':
    app.run(debug=True)
