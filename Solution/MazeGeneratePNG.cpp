#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "MazeGeneratePNG.h"

MazeGeneratePNG::MazeGeneratePNG(int height, int width)
{
	// height and width must be odd numbers for the maze to be "correct"
	if ((!(height % 2)) || (!(width % 2)))
	{
		throw std::invalid_argument("Height and width must be odd numbers!");
	}

	this->height = height;
	this->width = width;

	// create a grid with only walls
	grid.resize(height);
	for (int y = 0; y < height; y++)
	{
		grid[y].resize(width);
		for (int x = 0; x < width; x++)
		{
			grid[y][x] = wall;
		}
	}
}

MazeGeneratePNG::~MazeGeneratePNG()
{
}

void MazeGeneratePNG::SetCell(int y, int x, int value)
{
	if (((int)grid.size() <= y) || ((int)grid[y].size() <= x))
	{
		throw std::invalid_argument("Out of boundary!");
	}
	else if ((this->wall != value) && (this->path != value))
	{
		throw std::invalid_argument("The input can only contain 0 or 1.");
	}
	else
	{
		grid[y][x] = value;
	}
}

int MazeGeneratePNG::GetCell(int y, int x)
{
	return grid[y][x];
}

void MazeGeneratePNG::Generate(void)
{
	std::mt19937 random_generator(randomDevice());

	// set up sets
	sets.resize(grid.size());
	for (int y = 0; y < (int)grid.size(); y++)
	{
		sets[y].resize(grid[y].size());
	}

	// save every y, x coordinate with a possible movement (vertical or horizontal) and fill the sets with different values
	std::vector<Element> elements;
	int i = 1;
	for (int y = 1; y < ((int)sets.size() - 1); y += 2)
	{
		for (int x = 1; x < ((int)sets[0].size() - 1); x += 2)
		{
			if ((y + 2) < ((int)sets.size() - 1))
			{
				elements.push_back({ y, x, vertical });
			}
			if ((x + 2) < ((int)sets[0].size() - 1))
			{
				elements.push_back({ y, x, horizontal });
			}
			sets[y][x] = i;
			i++;
		}
	}

	// shuffle the elements vector
	std::shuffle(elements.begin(), elements.end(), random_generator);

	// do it until there are no elements left
	while (elements.size())
	{
		int y = elements[elements.size() - 1].y;
		int x = elements[elements.size() - 1].x;
		int orientation = elements[elements.size() - 1].orientation;
		elements.pop_back();

		if (horizontal == orientation)
		{
			// if the two sets are different, join them and carve a passage
			if (sets[y][x + 2] != sets[y][x])
			{
				Replace(sets[y][x + 2], sets[y][x]);
				for (int j = 0; j < 3; j++)
				{
					grid[y][x + j] = path;
				}
			}
		}
		else if (vertical == orientation)
		{
			// if the two sets are different, join them and carve a passage
			if (sets[y + 2][x] != sets[y][x])
			{
				Replace(sets[y + 2][x], sets[y][x]);
				for (int j = 0; j < 3; j++)
				{
					grid[y + j][x] = path;
				}
			}
		}
		else
		{
			// do nothing
		}
	}

	this->RemoveRandomWalls(this->width);
}

void MazeGeneratePNG::GenerateExit()
{
	bool foundExit = false;
	glm::vec2 exitPos(-1);

	while (!foundExit)
	{
		// Randomize exitPos
		bool horizontal = rand() % 2;
		bool below = rand() % 2;
		int random = rand() % (this->width - 1);

		if (horizontal && !below)
			exitPos = glm::vec2(random, 0);
		else if (horizontal && below)
			exitPos = glm::vec2(random, this->height - 1);
		else if (!horizontal && !below)
			exitPos = glm::vec2(0, random);
		else if (!horizontal && below)
			exitPos = glm::vec2(this->width - 1, random);

		// Check if horizontal exit

		// if below, need to make sure we dont read outside of the texture
		if (!below)
		{
			if (this->grid[exitPos.y + horizontal][exitPos.x + !horizontal] == this->path)
			{
				foundExit = true;
				break;
			}

		}
		else if (below)
		{
			if (this->grid[exitPos.y - horizontal][exitPos.x - !horizontal] == this->path)
			{
				foundExit = true;
				break;
			}
		}
		else //(this->grid[exitPos.y + horizontal][exitPos.x + !horizontal] == this->path && this->grid[exitPos.y - horizontal][exitPos.x - !horizontal] == this->path)
		{
			foundExit = true;
			break;
		}
	}

	// Set exit to floor
	this->grid[exitPos.y][exitPos.x] = this->path;
}

void MazeGeneratePNG::Replace(int setToReplace, int sampleSet)
{
	for (int search_y = 1; search_y < ((int)sets.size() - 1); search_y += 2)
	{
		for (int search_x = 1; search_x < ((int)sets[0].size() - 1); search_x += 2)
		{
			if (sets[search_y][search_x] == setToReplace)
			{
				sets[search_y][search_x] = sampleSet;
			}
		}
	}
}

void MazeGeneratePNG::RemoveRandomWalls(int NrOfWallsToRemove)
{
	int x;
	int y;

	unsigned int counter = 0;
	bool breaked = false;

	// Break 10 walls
	for (int i = 0; i < NrOfWallsToRemove && !breaked; i++)
	{
		// Run until we find a valid wall to break
		while (true)
		{
			// Find a position
			// Random number between 2 and 60
			x = rand() % (this->width - 4) + 2;
			y = rand() % (this->height - 4) + 2;

			// Check if the wall actually is a wall
			if (this->grid[y][x] == this->path)
			{
				bool closeby[4]; // false for floor
				// Read closeby pixels
				// 
				//    _0_
				// 3 | . | 1
				//   |___|
				//	   2

				// Assume its surrounded by walls
				closeby[0] = GetCell(y - 1, x);
				closeby[1] = GetCell(y, x + 1);
				closeby[2] = GetCell(y + 1, x);
				closeby[3] = GetCell(y, x - 1);

				if (closeby[0] && closeby[1] && closeby[2] && !closeby[3])
				{
					// Break down wall
					this->grid[y][x+1] = 0;
					break;
				}
				else if (closeby[0] && closeby[1] && !closeby[2] && closeby[3])
				{
					// Break down wall
					this->grid[y-1][x] = 0;
					break;
				}
				else if (closeby[0] && !closeby[1] && closeby[2] && closeby[3])
				{
					// Break down wall
					this->grid[y][x-1] = 0;
					break;
				}
				else if (!closeby[0] && closeby[1] && closeby[2] && closeby[3])
				{
					// Break down wall
					this->grid[y+1][x] = 0;
					break;
				}
			}

			// Prevent infinity loop
			counter++;
			if (counter > this->width * this->height)
			{
				
				breaked = true;
				break;
			}
		}
		counter = 0;
	}
}

void MazeGeneratePNG::DrawPNG()
{
	// Color png
	SetupColorDataForColor();

	std::vector<unsigned char> a;
	for (int y = 0; y < this->height+1; y++)
	{
		for (int x = 0; x < this->width+1; x++)
		{
			a.push_back(image[y][x][0]);
			a.push_back(image[y][x][1]);
			a.push_back(image[y][x][2]);
		}
	}
	
	if (!stbi_write_png("MazePNG/mazeColorCoded.png", this->width+1, this->height+1, 3, a.data(), 3 * (width+1)))
	{
		std::cout << "ERROR mazeColorCoded.png not written" << std::endl;
	}
	else
	{
		std::cout << "mazeColorCoded.png written to MazePNG-folder" << std::endl;
	}

	// Black-White png
	SetupColorData();

	std::vector<unsigned char> b;
	for (int y = 0; y < this->height+1; y++)
	{
		for (int x = 0; x < this->width+1; x++)
		{
			b.push_back(image[y][x][0]);
			b.push_back(image[y][x][1]);
			b.push_back(image[y][x][2]);
		}
	}

	if (!stbi_write_png("MazePNG/mazeBlackWhite.png", this->width+1, this->height+1, 3, b.data(), 3 * (this->width+1)))
	{
		std::cout << "ERROR mazeBlackWhite.png not written" << std::endl;
	}
	else
	{
		std::cout << "mazeBlackWhite.png written to MazePNG-folder" << std::endl;
	}
}

std::vector<std::vector<int>> MazeGeneratePNG::GetGrid()
{
	return this->grid;
}

void MazeGeneratePNG::SetupColorDataForColor()
{// set up image vectors to hold color data
	// added +1 to make the maze 64*64 pixels for use in shaders
	int newHeight = this->height + 1;
	int newWidth = this->width + 1;

	image.resize(newHeight);
	for (int y = 0; y < newHeight; y++)
	{
		image[y].resize(newWidth);
		for (int x = 0; x < newWidth; x++)
		{
			image[y][x].resize(3);
		}
	}

	for (int i = 0; i < newWidth; i++)
	{
		image[this->height][i][0] = 0;
		image[this->height][i][1] = 0;
		image[this->height][i][2] = 0;
	}
	for (int i = 0; i < newHeight; i++)
	{
		image[i][this->width][0] = 0;
		image[i][this->width][1] = 0;
		image[i][this->width][2] = 0;
	}


	// sets colors, white for walls, black for path
	for (int y = 0; y < this->height; y++)
	{
		for (int x = 0; x < this->width; x++)
		{
			if (GetCell(y, x) == wall)
			{
				bool closeby[4]; // false for floor
				// Read closeby pixels
				// 
				//    _0_
				// 3 | . | 1
				//   |___|
				//	   2

				// Assume its surrounded by walls
				closeby[0] = wall;
				closeby[1] = wall;
				closeby[2] = wall;
				closeby[3] = wall;

				// If at the edge, keep the assumption else check if there is a floor or wall
				if (y == 0)
					closeby[2] = GetCell(y + 1, x);
				else if (y == this->height - 1)
					closeby[0] = GetCell(y - 1, x);
				else
				{
					closeby[0] = GetCell(y - 1, x);
					closeby[2] = GetCell(y + 1, x);
				}

				if (x == 0)
					closeby[1] = GetCell(y, x + 1);
				else if (x == this->width - 1)
					closeby[3] = GetCell(y, x - 1);
				else
				{
					closeby[1] = GetCell(y, x + 1);
					closeby[3] = GetCell(y, x - 1);
				}

				// Check which wall type it is based on closeby
				if (closeby[0] && closeby[1] && closeby[2] && closeby[3]) // 0. Empty
				{
					image[y][x][0] = 75;
					image[y][x][1] = 75;
					image[y][x][2] = 75;
				}
				else if (!closeby[0] && closeby[1] && closeby[2] && closeby[3]) // 1. Single wall closeby
				{
					image[y][x][0] = 100;
					image[y][x][1] = 0;
					image[y][x][2] = 255;
				}
				else if (closeby[0] && closeby[1] && !closeby[2] && closeby[3]) // 2.
				{
					image[y][x][0] = 100;
					image[y][x][1] = 255;
					image[y][x][2] = 0;
				}
				else if (closeby[0] && !closeby[1] && closeby[2] && closeby[3]) // 3.
				{
					image[y][x][0] = 100;
					image[y][x][1] = 255;
					image[y][x][2] = 255;
				}
				else if (closeby[0] && closeby[1] && closeby[2] && !closeby[3]) // 4.
				{
					image[y][x][0] = 100;
					image[y][x][1] = 0;
					image[y][x][2] = 0;
				}
				else if (!closeby[0] && !closeby[1] && closeby[2] && closeby[3]) // 5. Corner walls
				{
					image[y][x][0] = 200;
					image[y][x][1] = 0;
					image[y][x][2] = 255;
				}
				else if (!closeby[0] && closeby[1] && closeby[2] && !closeby[3]) // 6.
				{
					image[y][x][0] = 200;
					image[y][x][1] = 255;
					image[y][x][2] = 0;
				}
				else if (closeby[0] && closeby[1] && !closeby[2] && !closeby[3]) // 7.
				{
					image[y][x][0] = 200;
					image[y][x][1] = 0;
					image[y][x][2] = 0;
				}
				else if (closeby[0] && !closeby[1] && !closeby[2] && closeby[3]) // 8.
				{
					image[y][x][0] = 200;
					image[y][x][1] = 255;
					image[y][x][2] = 255;
				}
				else if (!closeby[0] && !closeby[1] && closeby[2] && !closeby[3]) // 9. Tripple wall closeby
				{
					image[y][x][0] = 255;
					image[y][x][1] = 0;
					image[y][x][2] = 255;
				}
				else if (!closeby[0] && !closeby[1] && !closeby[2] && closeby[3]) // 10.
				{
					image[y][x][0] = 255;
					image[y][x][1] = 255;
					image[y][x][2] = 0;
				}
				else if (closeby[0] && !closeby[1] && !closeby[2] && !closeby[3]) // 11.
				{
					image[y][x][0] = 255;
					image[y][x][1] = 0;
					image[y][x][2] = 0;
				}
				else if (!closeby[0] && closeby[1] && !closeby[2] && !closeby[3]) // 12.
				{
					image[y][x][0] = 255;
					image[y][x][1] = 255;
					image[y][x][2] = 255;
				}
				else if (closeby[0] && !closeby[1] && closeby[2] && !closeby[3]) // 13. Double wall closeby Ex
				{
					image[y][x][0] = 50;
					image[y][x][1] = 0;
					image[y][x][2] = 0;
				}
				else if (!closeby[0] && closeby[1] && !closeby[2] && closeby[3]) // 14.
				{
					image[y][x][0] = 50;
					image[y][x][1] = 255;
					image[y][x][2] = 255;
				}
				else if (!closeby[0] && !closeby[1] && !closeby[2] && !closeby[3]) // 14. Nothing should never occur
				{
					image[y][x][0] = 177;
					image[y][x][1] = 177;
					image[y][x][2] = 177;
				}
			}
			else if (GetCell(y, x) == path)
			{
				for (int i = 0; i < 3; i++)
				{
					image[y][x][i] = 0;
				}
			}
		}
	}
}

void MazeGeneratePNG::SetupColorData()
{
	// set up image vectors to hold color data
	// added +1 to make the maze 64*64 pixels for use in shaders
	int newHeight = this->height + 1;
	int newWidth = this->width + 1;

	image.resize(newHeight);
	for (int y = 0; y < newHeight; y++)
	{
		image[y].resize(newWidth);
		for (int x = 0; x < newWidth; x++)
		{
			image[y][x].resize(3);
		}
	}
	// draw black border on bottom and left
	for (int i = 0; i < newWidth; i++)
	{
		image[this->height][i][0] = 0;
		image[this->height][i][1] = 0;
		image[this->height][i][2] = 0;
	}
	for (int i = 0; i < newHeight; i++)
	{
		image[i][this->width][0] = 0;
		image[i][this->width][1] = 0;
		image[i][this->width][2] = 0;
	}

	// sets colors, white for walls, black for path
	for (int y = 0; y < this->height; y++)
	{
		for (int x = 0; x < this->width; x++)
		{
			if (GetCell(y, x) == wall)
			{
				for(int i = 0; i < 3; i++)
				{
					image[y][x][i] = 255;
				}
			}
			else if (GetCell(y, x) == path)
			{
				for (int i = 0; i < 3; i++)
				{
					image[y][x][i] = 0;
				}
			}
		}
	}
}
