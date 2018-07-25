#include "obj.h"
#include <assert.h>
#include <fstream>
#include <sstream>
#include <iostream>

// ---------------------------------------- //
char int_to_str_out[100];
std::string int_to_str(const int x)
{
	sprintf(int_to_str_out, "%d", x);

	return std::string(int_to_str_out);
}
// ---------------------------------------- //


std::string get_pure_filename(std::string filename)
{
	unsigned int pos1 = filename.find_last_of("/");
	unsigned int pos2 = filename.find_last_of("\\");
	unsigned int pos3 = filename.find_last_of(".");

	if (pos1 == std::string::npos) pos1 = pos2;
	if (pos1 == std::string::npos) pos1 = 0;
	if (pos1 < filename.size())
		if (pos1 != 0)pos1++;

	if (pos3 == std::string::npos)
	{
		pos3 = filename.size();
	}
	//printf( "input %s substr = %s\n" ,filename.c_str(), filename.substr(pos1,pos3-pos1).c_str());
	return (filename.substr(pos1, pos3 - pos1));
}

std::string get_path(std::string filename)
{
	unsigned int pos1 = filename.find_last_of("/");
	unsigned int pos2 = filename.find_last_of("\\");

	if (pos1 == std::string::npos) pos1 = pos2;
	if (pos1 == std::string::npos) return "./";

	if (pos1 < filename.size())
		if (pos1 != 0)
			pos1++;

	//printf( "substr = %s\n" , filename.substr(0,pos1).c_str());

	return (filename.substr(0, pos1));
}

bool ReadOBJ(Mesh& mesh, const std::string& file_name)
{
	printf("\nLoading Objects %s ... \n", file_name.c_str());

	get_pure_filename(file_name);

	std::string path = get_path(file_name);
	FILE* fn;
	if (file_name.c_str() == NULL)		return false;
	if ((char)file_name[0] == 0)	return false;
	if ((fn = fopen(file_name.c_str(), "rb")) == NULL)
	{
		printf("File %s not found!\n", file_name.c_str());
		while (1);
	}

	char line[1000];
	memset(line, 0, 1000);

	int material = -1;
	int vertex_cnt = 0;
	int tex_coords_cnt = 0;
	int normals_cnt = 0;

	char state = '-', state_before = '-';

	static int counter = 0; counter++;

	std::string mtl_prefix = int_to_str(counter) + "-";

	while (fgets(line, 1000, fn) != NULL)
	{
		Vector3f val;
		Vector2f tal;
		int integers[9];

		if (line[0] == 'v')
		{
			if (line[1] == ' ')
			{
				if (sscanf(line, "v %f %f %f",
					&val[0], &val[1], &val[2]) == 3)
				{
					mesh.AddVertex(Vector3d(val[0],val[1],val[2]));
				}
			}
			else if (line[1] == 't')
			{
				if (sscanf(line, "vt %f %f",
					&tal[0], &tal[1]) == 2)
					mesh.AddTexture(Vector2d(tal[0], tal[1]));
			}
			else if (line[1] == 'n')
			{
				if (sscanf(line, "vn %f %f %f",
					&val[0], &val[1], &val[2]) == 3)
				{
					mesh.AddVertexNormal(Vector3d(val[0], val[1], val[2]));
				}
			}
		}
		else if (line[0] == 'f')
		{
			bool tri_ok = false;

			if (sscanf(line, "f %d %d %d",
				&integers[0], &integers[1], &integers[2]) == 3)
			{
				mesh.AddFace(Vector3i(integers[0] - 1, integers[1] - 1, integers[2] - 1));
			}
			else if (sscanf(line, "f %d// %d// %d//",
					&integers[0], &integers[1], &integers[2]) == 3)
			{
				mesh.AddFace(Vector3i(integers[0] - 1, integers[1] - 1, integers[2] - 1));
			}
			else if (sscanf(line, "f %d//%d %d//%d %d//%d",
					&integers[0], &integers[3],
					&integers[1], &integers[4],
					&integers[2], &integers[5]) == 6)
			{
				mesh.AddFace(Vector3i(integers[0] - 1, integers[1] - 1, integers[2] - 1));
				mesh.AddFaceNormalIndex(Vector3i(integers[3] - 1, integers[4] - 1, integers[5] - 1));
			}
			else if (sscanf(line, "f %d/%d %d/%d %d/%d",
				&integers[0], &integers[3],
				&integers[1], &integers[4],
				&integers[2], &integers[5]) == 6)
			{
				mesh.AddFace(Vector3i(integers[0] - 1, integers[1] - 1, integers[2] - 1));
				mesh.AddFaceCoordIndex(Vector3i(integers[3] - 1, integers[4] - 1, integers[5] - 1));
			}
			else if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
							&integers[0], &integers[6], &integers[3],
							&integers[1], &integers[7], &integers[4],
							&integers[2], &integers[8], &integers[5]) == 9)
			{
				mesh.AddFace(Vector3i(integers[0] - 1, integers[1] - 1, integers[2] - 1));
				mesh.AddFaceNormalIndex(Vector3i(integers[3] - 1, integers[4] - 1, integers[5] - 1));
				mesh.AddFaceCoordIndex(Vector3i(integers[6] - 1, integers[7] - 1, integers[8] - 1));
			}
			else
			{
				printf("unrecognized sequence\n");
				printf("%s\n", line);
				while (1);
			}
		}
		
	}
	return true;
}