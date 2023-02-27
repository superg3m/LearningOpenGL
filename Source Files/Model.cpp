#include <Headers/model.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>

enum FILE_STATE
{
	no_state,
	v,
	vn,
	vt,
	f
};

void Model::loadModel(const std::string modelPath)
{
	std::ifstream file;
	std::vector<std::string> file_data;
	std::stringstream ss;
	FILE_STATE fileState = no_state;
	int iteration_until_new_state = 0;
	std::vector<std::string> dataCache;
	glm::vec3 verts;
	bool lock = false;
	file.open(modelPath);
	if (!file) 
	{
		std::cerr << "Model path failed to load" << "\n";
	}
	while(file.good())
	{
		std::string line;
		std::getline(file, line);
		file_data.push_back(line);
	}
	for (std::string s : file_data)
	{
		std::string lineSubString;
		ss.str(s);
		
		while (ss >> lineSubString)
		{
			std::cout << lineSubString << "\n";
			if (lineSubString == "v")
			{
				fileState = v;
				continue;
			}
				
			else if (lineSubString == "vn") fileState = vn;
			else if (lineSubString == "vt") fileState = vt;
			else if (lineSubString == "f") fileState = f;

			/*
			switch (fileState)
			{
			case no_state:
				std::cout << "File_State: no_state" << "\n";
				break;
			case v:
				std::cout << "File_State: v" << "\n";
				break;
			case vn:
				std::cout << "File_State: vn" << "\n";
				break;
			case vt:
				std::cout << "File_State: vt" << "\n";
				break;
			case f:
				std::cout << "File_State: f" << "\n";
				break;
			default:
				break;
			}
			*/
			

			if (fileState == v)
			{
				//std::cout << lineSubString << "\n";
				dataCache.push_back(lineSubString);
				iteration_until_new_state++;
			}
			if (fileState == vn)
			{
				//dataCache.push_back(component);
			}
			if (fileState == vt)
			{
				//dataCache.push_back(component);
			}
			if (fileState == f)
			{
				//dataCache.push_back(component);
			}
			

			if (fileState == v && iteration_until_new_state == 3)
			{
				std::cout << "Original: " << s << "\n";
				std::cout << "X: " << dataCache[0] << " | Y: " << dataCache[1] << " | Z: " << dataCache[2];
				//verts.x = std::stod(dataCache[0]);
				//verts.y = std::stod(dataCache[1]);
				//verts.z = std::stod(dataCache[2]);
				//this->vertcies.push_back(verts);
				verts = glm::vec3(0.0f);
				dataCache.clear();
				iteration_until_new_state = -1;
			}
		contin:;
		}
		ss.clear();
	}
	for (int i = 0; i < this->vertcies.size(); i++)
	{
		//std::cout << "X: " << this->vertcies[i].x << " | Y: " << this->vertcies[i].y << " | Z: " << this->vertcies[i].z;
	}
}

