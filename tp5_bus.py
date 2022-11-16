from flask import Flask, jsonify, abort, render_template, request
import json
import serial
app = Flask(__name__)

uart = serial.Serial('/dev/ttyAMA0', 115200, timeout=1)

temp = []
pres = []
K = 10
angle = 90

@app.route('/')
def hello_world():
    return 'Hello, World!\n'

@app.route('/temp/', methods = ['POST', 'GET'])
def postTemp():
    uart.write(b"GET_T\r")
    temperature = (uart.readline().strip()).decode('ascii')
    temperature = float(temperature)
    temp.append(temperature)
    if request.method == 'POST':
        return "", 202
    return jsonify({"Valeurs temperature" : temperature})

@app.route('/pres/', methods = ['POST'])
def postPres(path = '/pres/'):
    resp = {
        "method": request.method,
        "url" :  request.url,
        "path" : path,
        "args": request.args,
        "headers": dict(request.headers),
    }
    if request.method == 'POST':
        resp["POST"] = {
            "data" : request.get_json(),
        }
        json = request.get_json()
        pres.append(json["pres"])
    return jsonify(resp)


@app.route('/temp/', methods = ['GET'])
def getTemp():
    list = []
    for index in range(len(temp)):
        list.append({"index" : index, "value" : temp[index]})
    total = {"all" : list}
    return jsonify(total)

@app.errorhandler(404)
def page_not_found(error):
    return render_template('page_not_found.html'), 404

@app.route('/pres/', methods = ['GET'])
def getPres():
    list = []
    for index in range(len(pres)):
        list.append({"index" : index, "value" : pres[index]})
    total = {"all" : list}
    return jsonify(total)


@app.route('/pres/<int:index>', methods = ['GET'])
def getPresIndex(index):
    if (index > len(pres)):
        abort(404)
    else :
        total = {"index" : index, "value" : pres[index]}
        return jsonify(total)

@app.route('/temp/<int:index>', methods = ['GET'])
def getTempIndex(index):
    if (index > len(temp)):
        abort(404)
    else :
        total = {"index" : index, "value" : temp[index]}
        return jsonify(total)

@app.route('/temp/<int:index>', methods = ['DELETE'])
def deleteTempIndex(index):
    if (index > len(temp)):
        abort(404)
    else : 
        temp.pop(index)
        return "The data is deleted\r\n"
    
@app.route('/pres/<int:index>', methods = ['DELETE'])
def deletePresIndex(index):
    if (index > len(pres)):
        abort(404)
    else : 
        pres.pop(index)
        return "The data is deleted\r\n"

@app.route('/scale/', methods = ['GET'])
def getScale():
    constante = {"Coefficient de proportionnalite K" : K}
    return jsonify(constante)


@app.route('/angle/', methods = ['GET'])
def getAngle():
    constante = {"Angle" : angle}
    return jsonify(constante)


@app.route('/scale/<int:value>', methods = ['POST'])
def postScale(value):
    global K
    if request.method == 'POST':
        K  = value
    return jsonify({"Coeff" : K})


