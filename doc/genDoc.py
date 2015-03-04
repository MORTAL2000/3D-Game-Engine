import sys
import json

#from xml.dom import minidom
#xmldoc = minidom.parse('items.xml')
#itemlist = xmldoc.getElementsByTagName('item')
#print len(itemlist)
#print itemlist[0].attributes['name'].value
#for s in itemlist :
#    print s.attributes['name'].value

class Parameter():
	def __init__(self):
		self.type = "nil"
		self.name = "nil"
		self.desc = "Description is not available"

	def setType(self, type):
		self.type = type

	def setName(self, name):
		self.name = name

	def setDescription(self, desc):
		self.desc = desc

class Property():
	def __init__(self):
		self.name = "nil"
		self.type = "nil"
		self.desc = "Description is not available"

	def setName(self, name):
		self.name = name

	def setType(self, type):
		self.type = type

	def setDescription(self, desc):
		self.desc = desc

class Method():
	def __init__(self):
		self.name = "nil"
		self.type = "nil"
		self.desc = "Description is not available"
		self.params = []

	def setName(self, name):
		self.name = name

	def setType(self, type):
		self.type = type

	def setDescription(self, desc):
		self.desc = desc

	def addParam(self, param):
		self.params.append(param)

class Class():
	def __init__(self, filename):
		self.methods = []
		self.properties = []
		self.name = "nil"
		self.doc = open(filename, "wb")
		self.desc = "Description is not available"

	def println(self, text):
		self.doc.write(text+'\n')

	def printn(self, text):
		self.doc.write(text)

	def newline(self):
		self.doc.write('\n')

	def setName(self, name):
		self.name = name

	def setDescription(self, desc):
		self.desc = desc

	def addMethod(self, method):
		self.methods.append(method)

	def addProperty(self, prop):
		self.properties.append(prop)

	def createHTML(self):
		#header
		self.println("<html>")
		self.println("<head>")
		self.println("\t<!-- Created using genDoc.py, Copyright(c), 2015 Alexander Koch -->")
		self.println("\t<link rel=\"stylesheet\" type=\"text/css\" media=\"screen\" href=\"main.css\"/>")
		self.println("\t<link href=\"lua_doc.css\" type=\"text/css\" rel=\"stylesheet\" />")
		self.println("\t<link rel=\"stylesheet\" href=\"roboto.css\" type=\"text/css\">")
		self.println("</head>")
		self.println("<body>")

		self.println("\t<div id=\"content_pane\">")

		self.println("\t<div class=\"feature\">")
		self.println("\t\t<div class=\"feature_txt\">")
		self.println("\t\t\t<h1>Game engine class documentation</h1>")
		self.println("\t\t\t<p>Copyright(c), 2015 Alexander Koch</p>")
		self.println("\t\t</div>")
		self.println("\t</div>")
		self.newline()

		#self.println("\t<div id=\"nav\">")
		#self.println("\t\t<div id=\"nav_menu\">")
		#self.println("\t\t\t<span class=\"menu_item\"><a href=\"classList.html\">Overview</a></span>")
		#self.println("\t\t</div>")
		#self.println("\t</div>")
		#self.newline()

		self.println("\t<div id=\"page_content_main\">")
		self.println("\t<div id=\"lua_docs_content\">")
		#<div id="class_list">
		self.newline()

		#body
		self.println("\t<div class=\"class_main\">")
		self.println("\t<div class=\"class_name\">"+self.name+"</div>")
		self.println("\t<div class=\"class_desc\">"+self.desc+"</div>")
		self.newline()

		if len(self.properties) > 0:
			self.println("\t<div class=\"class_properties\">")
			self.println("\t\t<div class=\"class_properties_title\">Properties</div>")
			self.println("\t\t<div class=\"class_properties_list\">")
			self.println("\t\t\t<div class=\"class_property\">")
			for prop in self.properties:
				self.println("\t\t\t\t<div class=\"class_property_name\">"+prop.name+"</div>")
				self.println("\t\t\t\t<div class=\"class_property_type\">"+prop.type+"</div>")
				self.println("\t\t\t\t<div class=\"class_property_desc\">"+prop.desc+"</div>")

			self.println("\t\t\t</div>")
			self.println("\t\t</div>")
			self.println("\t</div>")
			self.newline()

		self.println("\t<div class=\"class_methods\">")
		self.println("\t\t<div class=\"class_methods_title\">Functions</div>")
		self.println("\t\t<div class=\"class_methods_list\">")
		self.newline()

		for method in self.methods:
			self.println("\t\t\t<div class=\"class_method\">")
			self.printn("\t\t\t\t<div class=\"class_method_name\">"+method.name)

			for param in method.params:
				self.printn(" ( <span class=\"inline_type\">"+param.type+"</span> <span class=\"inline_param\">"+param.name+"</span> )")

			if len(method.params) < 1:
				self.printn(" ( )")

			self.printn(" </div>\n")
			self.println("\t\t\t\t<div class=\"class_method_type\">"+method.type+"</div>")
			self.println("\t\t\t\t<div class=\"class_method_desc\">"+method.desc+"</div>")

			if len(method.params) > 0:
				self.println("\t\t\t\t<div class=\"class_method_params\">")
				self.println("\t\t\t\t<div class=\"class_method_params_title\">Parameters</div>")
				self.println("\t\t\t\t<div class=\"class_method_param\">")

				for param in method.params:
					self.println("\t\t\t\t\t<div class=\"class_method_param_name\">"+param.name+"</div>")
					self.println("\t\t\t\t\t<div class=\"class_method_param_type\">"+param.type+"</div>")
					self.println("\t\t\t\t\t<div class=\"class_method_param_desc\">"+param.desc+"</div>")

				self.println("\t\t\t\t</div>")
				self.println("\t\t\t\t</div>")

			self.println("\t\t\t</div>")
			self.newline()

		#footer
		self.println("\t\t</div>")
		self.println("\t</div>")
		self.newline()

		self.println("\t</div>")
		self.println("\t</div>")
		self.println("\t</div>")
		self.println("\t</div>")
		self.println("</body>")
		self.println("</html>")

def main():
	if len(sys.argv) != 3:
		print("Usage : <filename> <output>")
		sys.exit(2)
	fname = sys.argv[1]
	output = sys.argv[2]

	cclass = Class(output)
	with open(fname, "rb") as f:
		data = json.load(f)
		cclass.setName(data["name"])
		cclass.setDescription(data["desc"])

		if "properties" in data:
			for prop in data["properties"]:
				prop0 = Property()
				prop0.setName(prop["name"])
				prop0.setType(prop["type"])
				prop0.setDescription(prop["desc"])
				cclass.addProperty(prop0)

		if "methods" in data:
			for method in data["methods"]:
				func0 = Method()
				func0.setName(method["name"])
				func0.setType(method["type"])
				func0.setDescription(method["desc"])

				if "params" in method:
					for param in method["params"]:
						param0 = Parameter()
						param0.setName(param["name"])
						param0.setType(param["type"])
						param0.setDescription(param["desc"])
						func0.addParam(param0)

				cclass.addMethod(func0)

	cclass.createHTML()

if __name__ == '__main__':
	main()
