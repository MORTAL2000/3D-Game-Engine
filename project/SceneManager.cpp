#include "SceneManager.h"
using namespace StringUtils;

SceneManager SceneManager::getInstance()
{
    static SceneManager instance;
    return instance;
}

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::load(const std::string& filename)
{
    LuaScript script(filename);
    auto entities = script.getTableKeys("level.entities");
    for(auto i = 0; i < entities.size(); i++)
    {
        auto key = entities[i];
        auto entries = script.getTableKeys("level.entities."+key);

        auto shape_type = script.get<std::string>("level.entities."+key+".gfx.type");
        auto mat = script.get<std::string>("level.entities."+key+".gfx.mat");
        auto gfx_scale = script.getNumberVector("level.entities."+key+".gfx.scale");
        auto gfx_rotation = script.getNumberVector("level.entities."+key+".gfx.rotation");
        auto gfx_cull_face = script.get<std::string>("level.entities."+key+".gfx.cull_face");

        int raw_cull_face = atoi(gfx_cull_face.c_str());
        std::string cull_face = "back";
        if(raw_cull_face == GL_FRONT)
        {
            cull_face = "front";
        }
        else if(raw_cull_face == GL_BACK)
        {
            cull_face = "back";
        }
        else if(raw_cull_face == GL_NONE)
        {
            cull_face = "none";
        }

        quat rotation(gfx_rotation[0], gfx_rotation[1], gfx_rotation[2], gfx_rotation[3]);
        vec3 euler = glm::eulerAngles(rotation);

        // begin commands
        std::string cmd = "scene = Scene()\n";
        cmd.append(format_str("m%d = Mesh()\n", i));
        if(shape_type == "Mesh")
        {
            // source is the file path
            auto source = script.get<std::string>("level.entities."+key+".gfx.source");
            cmd.append(format_str("m%d:load(\"%s\")\n", i, source.c_str()));
        }
        else if(shape_type == "Cube")
        {
            cmd.append(format_str("m%d:loadCube(1.0)\n", i));
        }
        else if(shape_type == "Plane")
        {
            cmd.append(format_str("m%d:loadPlane(1.0)\n", i));
        }
        cmd.append(format_str("m%d:setScale({%f, %f, %f})\n", i, gfx_scale[0], gfx_scale[1], gfx_scale[2]));
        cmd.append(format_str("m%d:setRotation({%f, %f, %f})\n", i, euler.x, euler.y, euler.z));

        if(mat != "default")
        {
            cmd.append(format_str("m%d:setMaterial(Material(\"%s\"))\n", i, mat.c_str()));
        }
        cmd.append(format_str("m%d:setCullFace(\"%s\")\n", i, cull_face.c_str()));

        auto position = script.getNumberVector("level.entities."+key+".gfx.position");
        if(entries.size() > 1 && std::find(entries.begin(), entries.end(), "phys") != entries.end())
        {
            // Physics
            auto phys_shape = script.get<std::string>("level.entities."+key+".phys.shape");
            auto mass = script.get<float>("level.entities."+key+".phys.mass");
            auto friction = script.get<float>("level.entities."+key+".phys.friction");
            auto restitution = script.get<float>("level.entities."+key+".phys.restitution");

            if(phys_shape == "Sphere")
            {
                float scale = script.get<float>("level.entities."+key+".phys.scale");
                cmd.append(format_str("b%d = RigidBody(Shape.%s, %f)\n", i, phys_shape.c_str(), scale));
            }
            else if(phys_shape == "Box")
            {
                std::vector<float> scale = script.getNumberVector("level.entities."+key+".phys.scale");
                cmd.append(format_str("b%d = RigidBody(Shape.%s, { %f, %f, %f })\n", i, phys_shape.c_str(), scale[0], scale[1], scale[2]));
            }
            else
            {
                cmd.append(format_str("b%d = RigidBody(Shape.%s)\n", i, phys_shape.c_str()));
            }

            cmd.append(format_str("b%d:setMass(%f)\n", i, mass));
            cmd.append(format_str("b%d:setFriction(%f)\n", i, friction));
            cmd.append(format_str("b%d:setRestitution(%f)\n", i, restitution));
            cmd.append(format_str("b%d:setMesh(m%d)\n", i, i));
            cmd.append(format_str("b%d:create({0, 0, 0, 1}, {%f,%f,%f})\n", i, position[0], position[1], position[2]));
            cmd.append(format_str("scene:addEntity(b%d)\n", i));
        }
        else
        {
            cmd.append(format_str("m%d:setPosition({%f, %f, %f})\n", i, position[0], position[1], position[2]));
        }
        cmd.append(format_str("scene:addEntity(m%d)\n", i));
        LuaAPI::getInstance().runCommand(cmd);
    }
    script.close();
}

bool SceneManager::exportScene(const std::string& path, SceneNode* scene)
{
    std::string filename = path;
    std::replace(filename.begin(), filename.end(), '\\', '/');

    FILE* file = fopen(filename.c_str(), "wb");
    if(!file) return false;
    fprintf(file, "level = {\n");
    fprintf(file, "\tentities = {\n");

    SceneNode::NodePointerList nodes = scene->getChildren();
    for(auto i = 0; i < nodes.size(); i++)
    {
        auto node = dynamic_cast<RenderableNode*>(nodes[i]);
        if(!node) continue;

        auto renderer = node->getRenderer();
        auto name = node->getName();

        // gfx
        std::string shape_type = RenderTypes::Typenames[node->getType()];
        vec3 gfx_scale = renderer->getScale();
        quat gfx_rotation = renderer->getRotation();
        int cull_face = renderer->getCullFace();

        std::string source = "None";
        if(node->getType() == RenderableNode::Type::MESH)
        {
            source = node->getMeshPath();
            std::replace(source.begin(), source.end(), '\\', '/');
        }

        std::string mat = "default";
        UserMaterial* material = dynamic_cast<UserMaterial*>(renderer->getMaterial());
        if(material)
        {
            mat = material->getName();
        }

        // physics
        RenderableNode::Collision phys_type = node->getCollisionType();
        PhysicsInfo phys = node->getPhysicsInfo();

        std::string phys_shape = ColliderTypes::ColliderNames[node->getColliderType()];
        vec3 position = renderer->getPosition();
        vec3 scale = phys.scale;

        double mass = phys.mass;
        double friction = phys.friction;
        double restitution = phys.restitution;

        fprintf(file, "\t\t%s = {\n", name.c_str());
        fprintf(file, "\t\t\tgfx = {\n");
        fprintf(file, "\t\t\t\ttype = \"%s\",\n", shape_type.c_str());
        fprintf(file, "\t\t\t\tsource = \"%s\",\n", source.c_str());
        fprintf(file, "\t\t\t\tscale = {%f, %f, %f},\n", gfx_scale.x, gfx_scale.y, gfx_scale.z);
        fprintf(file, "\t\t\t\trotation = {%f, %f, %f, %f},\n", gfx_rotation.w, gfx_rotation.x, gfx_rotation.y, gfx_rotation.z);
        fprintf(file, "\t\t\t\tposition = {%f, %f, %f},\n", position.x, position.y, position.z);
        fprintf(file, "\t\t\t\tmat = \"%s\",\n", mat.c_str());
        fprintf(file, "\t\t\t\tcull_face = %d\n", cull_face);

        if(phys_type > 0)
        {
            if(phys_type == RenderableNode::Collision::STATIC) mass = 0.0;

            fprintf(file, "\t\t\t},\n");
            fprintf(file, "\t\t\tphys = {\n");
            fprintf(file, "\t\t\t\tshape = \"%s\",\n", phys_shape.c_str());
            if(phys_shape == "Sphere")
            {
                fprintf(file, "\t\t\t\tscale = %f,\n", scale.x);
            }
            else if(phys_shape == "Box")
            {
                fprintf(file, "\t\t\t\tscale = {%f, %f, %f},\n", scale.x, scale.y, scale.z);
            }
            fprintf(file, "\t\t\t\tmass = %f,\n", mass);
            fprintf(file, "\t\t\t\tfriction = %f,\n", friction);
            fprintf(file, "\t\t\t\trestitution = %f\n", restitution);
            fprintf(file, "\t\t\t}\n");
        }
        else
        {
            fprintf(file, "\t\t\t}\n");
        }

        if(i == nodes.size()-1) fprintf(file, "\t\t}\n");
        else fprintf(file, "\t\t},\n");
    }
    fprintf(file, "\t}\n");
    fprintf(file, "}\n");

    fclose(file);
}
