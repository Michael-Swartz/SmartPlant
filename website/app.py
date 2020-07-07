from flask import Flask, render_template
app = Flask(__name__)

@app.route('/button')
def button():
    print("Hello World")
    return render_template('main.html')


@app.route('/')
def start():
    return render_template('main.html')

if __name__ == '__main__':
    app.run()