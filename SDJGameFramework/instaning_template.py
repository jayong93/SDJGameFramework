from mako.template import Template
import io
import json

data = None
with io.open("template\\test.json") as d:
    data = json.loads(d.read())

header_tmp = Template(filename="template\\Components.h.in", output_encoding='utf-8')
body_tmp = Template(filename="template\\Components.cpp.in", output_encoding='utf-8')

print("make Components.h")
with io.open("Components.h", "wb") as header:
    result = header_tmp.render(data=data)
    header.write(result)
    print("Complete")

print("------------------------------------------------")

print("make Components.cpp")
with io.open("Components.cpp", "wb") as body:
    result = body_tmp.render(data=data)
    body.write(result)
    print("Complete")