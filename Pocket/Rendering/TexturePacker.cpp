#include "TexturePacker.hpp"
#include "lodepng.h"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace Pocket;
using namespace std;

TexturePacker::TexturePacker() {
}

TexturePacker::~TexturePacker() {
}

bool TexturePacker::CreateAtlas(std::string dir, std::string pngFile, std::string xmlFile, int maxWidth, int maxHeight) {

	string extension = ".png";

	vector<string> files;
	if (getdir(dir, files)) {
		return false;
	}

	vector<Image*> images;

	vector<TSubRect> rects;

	for (int i = 0; i<files.size(); i++) {
		string file = files[i];

		if (file.size()<extension.size()) continue;
		string fileExt = file.substr(file.size() - extension.size(), extension.size());
		if (fileExt!=extension) continue;

		Image* image = new Image();
		image->Load(file);
		images.push_back(image);

		string path = file.substr((dir.size() + 1), file.size() - (dir.size() + 1));
		path = path.substr(0, path.size() - extension.size());

		image->path = path;
		rects.push_back(TSubRect(image->width, image->height, i));
	}

	std::sort(rects.begin(), rects.end(), RectPacker::TRect::Greater);

	RectPacker packer;

	int border = 1;

	for (int i=0; i<rects.size(); i++) {
		Image* image = images[rects[i].n];

		int bw = image->width < maxWidth ? border : 0;
		int bh = image->height < maxHeight ? border : 0;

		RectPacker::TRect rect(0,0,image->width + bw, image->height + bh);
		image->willFit = packer.AddAtEmptySpotAutoGrow(&rect, maxWidth, maxHeight);

		image->x = rect.x;
		image->y = rect.y;
	}

	int atlasWidth = packer.GetW();
	int atlasHeight = packer.GetH();

	vector<unsigned char> atlas;
	int atlasSize = atlasWidth * atlasHeight * 4;
	
	for (int i=0; i<atlasSize; i++) {
		atlas.push_back(0);
	}

	for (int i=0; i<images.size(); i++) {
		Image* image = images[i];
		if (!image->willFit) continue;
		WritePixels(atlas, atlasWidth, atlasHeight, image->image, image->x, image->y, image->width, image->height);
	}

	std::vector<unsigned char> out;
		
	LodePNG::Encoder encoder;
	encoder.encode(out, &atlas[0], atlasWidth, atlasHeight);

	LodePNG::saveFile(out, pngFile);

	WriteXml(xmlFile, images);

	for (int i=0; i<images.size(); i++) {
		delete images[i];
	}

	

	return true;
}

void TexturePacker::WritePixels(std::vector<unsigned char>& buffer, int buffer_w, int buffer_h, std::vector<unsigned char>& pixels, int x, int y, int w, int h) {

	for (int yy=0; yy<h; yy++) {
		for (int xx=0; xx<w; xx++) {
			int dest = ConvertIndex(buffer_w, buffer_h, xx+x,yy+y);
			int source = xx * 4 + yy * w * 4;
			if (dest>=buffer.size()) {
				return;
			}

			buffer[dest] = pixels[source];
			buffer[dest+1] = pixels[source+1];
			buffer[dest+2] = pixels[source+2];
			buffer[dest+3] = pixels[source+3];
		}
	}

}

int TexturePacker::ConvertIndex(int buf_w, int buf_h, int x, int y) {
	return buf_w * y * 4 + x * 4;
}

void TexturePacker::Image::Load(std::string filename) {

	std::vector<unsigned char> buffer;
		
	LodePNG::loadFile(buffer, filename);
	LodePNG::Decoder decoder;

	decoder.decode(image, buffer);

	width = decoder.getWidth();
	height = decoder.getHeight();
}

int TexturePacker::getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        //cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
		string name =  string(dirp->d_name);
		if (name == "." || name == "..") continue;

		if (dirp->d_type == DT_DIR) {
			getdir(dir+"/"+name, files);
		}
		else {
			files.push_back(dir+"/"+name);
		}
    }
    closedir(dp);
    return 0;
}

void TexturePacker::WriteXml(std::string filename, vector<Image*>& images) {



	std::stringstream xml;

	xml << "<?xml version=\"1.0\" ?>";
	xml << "\n<Texture>\n";

	for (int n=0; n<images.size(); n++) {

		xml << "<Image";

		xml << " name=\"";
		xml <<images[n]->path<<"\"";

		xml << " position=\"";
		xml <<images[n]->x<<","<<images[n]->y<<"\"";

		xml << " size=\"";
		xml <<images[n]->width<<","<<images[n]->height<<"\"";

		xml << "/>\n";
	} 

	
	xml << "</Texture>";

	std::string xmlText = xml.str();
	
	std::ofstream file(filename.c_str());//, std::ios::out|std::ios::binary);
    
	file<<xmlText;
	
	file.close();

	//file.write(xmlText.empty() ? 0 : (char*)&xmlText[0], std::streamsize(xmlText.size()));

	
}