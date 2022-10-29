from flask import Flask, jsonify, abort, render_template, request
import json
app = Flask(__name__)

@app.route('/')
def hello_world():
    return 'Hello, World!\n'

welcome = "Welcome to 3ESE API!"

@app.route('/api/welcome/')
def api_welcome():
    return welcome + "\r\n"

@app.route('/api/welcome/<int:index>', methods=['GET'])
def api_welcome_index(index):
        if index < len(welcome):
                return jsonify({"index": index, "val": welcome[index]})
        else :
                abort(404)

@app.errorhandler(404)
def page_not_found(error):
    return render_template('page_not_found.html'), 404

@app.route('/api/request', methods=['GET', 'POST'])
@app.route('/api/request/<path>', methods=['GET','POST'])
def api_request(path=None):
    resp = {
            "method":   request.method,
            "url" :  request.url,
            "path" : path,
            "args": request.args,
            "headers": dict(request.headers),
    }
    if request.method == 'POST':
        resp["POST"] = {
                "data" : request.get_json(),
                }
    return jsonify(resp)

    @app.route('/api/welcome/<int:index>', methods=['PUT'])
def api_welcome_put_index(index):
        global welcome
        data = request.get_json()
        if index > len(welcome):
                abort(404)
        else :
                welcome = welcome[:index] + data + welcome[index:] + "\r\n"
                return welcome + "\r\n"

@app.route('/api/welcome/<int:index>', methods=['PATCH'])
def api_welcome_patch_index(index):
        global welcome
        data = request.get_json()
        if index >len(welcome) or len(data) != 1 :
                abort(404)
        else :
                temp = list(welcome)
                temp[index] = data
                welcome = "".join(temp)
                return welcome + "\r\n"

@app.route('/api/welcome/<int:index>', methods=['DELETE'])
def api_welcome_delete_index(index):
        global welcome
        if index > len(welcome):
                abort(404)
        else :
                temp = list(welcome)
                temp.pop(index)
                welcome = "".join(temp)
                return welcome + "\r\n"

@app.route('/api/welcome/', methods=['DELETE'])
def api_welcome_delete():
        global welcome
        welcome = ""
        return welcome + "\r\n"

@app.route('/api/welcome/', methods=['POST'])
def api_welcome_post():
        global welcome
        if request.method == 'POST':
                welcome = request.get_json()
        return welcome + "\r\n"
