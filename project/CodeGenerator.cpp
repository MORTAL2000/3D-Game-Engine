#include "CodeGenerator.h"

CodeGenerator::CodeGenerator() : m_cubes(0), m_spheres(0), m_planes(0)
{

}

CodeGenerator::~CodeGenerator()
{

}

void CodeGenerator::writeln(FILE* file, const std::string& content)
{
	fprintf(file, "%s\n", content.c_str());
}

void CodeGenerator::writeln(FILE* file)
{
	fprintf(file, "\n");
}

double CodeGenerator::readNumber(UINode* node, int index)
{
	double value = 0;
	NumberNode* number_ptr = dynamic_cast<NumberNode*>(node->getInputConnection(index));
	if(number_ptr)
	{
		value = number_ptr->getNumber();
	}
	return value;
}

double CodeGenerator::readNumber(UINode* node, const std::string& index)
{
	double value = 0;
	NumberNode* number_ptr = dynamic_cast<NumberNode*>(node->getInputConnection(index));
	if(number_ptr)
	{
		value = number_ptr->getNumber();
	}

	return value;
}

vec3 CodeGenerator::readVec3(UINode* node, const std::string& index)
{
	vec3 v(0.0);
	UINode* vector_ptr = node->getInputConnection(index);
	if(!vector_ptr)
	{
		return v;
	}

	if(vector_ptr->getType() != UINode::TYPE_VECTOR)
	{
		return v;
	}

	for(int i = 0; i < 3; i++)
	{
		v[i] = readNumber(vector_ptr, i);
	}

	return v;
}

bool CodeGenerator::generate(FILE* file, UINode* node, int index)
{
	UINode* node0 = node->getInputConnection(index);
	if(!node0)
	{
		return false;
	}

	switch(node0->getType())
	{
		case UINode::TYPE_MESH_CUBE:
		{
			double scale = readNumber(node0, "Scale");
			vec3 v = readVec3(node0, "Position");

			fprintf(file, "\tcube%d = Mesh()\n", m_cubes);
			fprintf(file, "\tcube%d:loadCube(%.4f)\n", m_cubes, scale);
			fprintf(file, "\tcube%d:setPosition({%.4f, %.4f, %.4f})\n", m_cubes, v.x, v.y, v.z);
			fprintf(file, "\tscene:addEntity(cube%d)\n", m_cubes);
			writeln(file);

			m_cubes++;
			return true;
		}
		case UINode::TYPE_MESH_PLANE:
		{
			double scale = 0;
			NumberNode* scale_ptr = dynamic_cast<NumberNode*>(node0->getInputConnection("Scale"));
			if(scale_ptr)
			{
				scale = scale_ptr->getNumber();
			}

			fprintf(file, "\tplane%d = Mesh()\n", m_planes);
			fprintf(file, "\tplane%d:loadPlane(%.4f)\n", m_planes, scale);
			fprintf(file, "\tscene:addEntity(plane%d)\n", m_planes);
			writeln(file);

			m_planes++;
			return true;
		}
		case UINode::TYPE_MESH_SPHERE:
		{
			double radius = 0;
			NumberNode* radius_ptr = dynamic_cast<NumberNode*>(node0->getInputConnection("Radius"));
			if(radius_ptr)
			{
				radius = radius_ptr->getNumber();
			}

			fprintf(file, "\tsphere%d = Mesh()\n", m_spheres);
			fprintf(file, "\tsphere%d:loadSphere(%.4f)\n", m_spheres, radius);
			fprintf(file, "\tscene:addEntity(sphere%d)\n", m_spheres);
			writeln(file);

			m_spheres++;
			return true;
		}
		case UINode::TYPE_MESH_ARRAY:
		{
			for(int i = 0; i < 6; i++)
			{
				generate(file, node0, i);
			}
			return true;
		}
		default:
		{
			Console::log("Type is not implemented");
			return false;
		}
	}

	Console::log("Failed to compile code");
	return false;
}

bool CodeGenerator::createCode(UINode* node)
{
	FILE* file = fopen("generated.lua", "wb");
	if(!file)
	{
		return false;
	}

	writeln(file, "scene = Scene()");
	writeln(file);
	writeln(file, "function onInit()");
	generate(file, node, 0);
	writeln(file, "end");

	fclose(file);
	return true;
}
