from mako.template import Template
import io
import json
import sys
import os.path

args = sys.argv

if len(args) <= 1:
    print("not enough arguments")
    exit()

data = None
with io.open("template\\Components.json") as d:
    data = json.loads(d.read())

tmp = Template(filename=str(args[1]), output_encoding='utf-8')
input_name = os.path.basename(str(args[1]))
output_name = os.path.splitext(input_name)[0]
result = tmp.render(data=data)

with io.open(output_name, "wb") as header:
    header.write(result)
    print("Complete Instancing", input_name, "to", output_name)
