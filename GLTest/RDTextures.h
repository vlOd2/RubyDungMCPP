#pragma once

#include "pch.h"
#include <glad/glad.h>
#include <corecrt_malloc.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

public ref class RDTextures abstract sealed {
private:
	static Dictionary<String^, int>^ idMap;

public:
	static unsigned int LoadTexture(String^ resourceName, int mode) {
		try {
			if (idMap->ContainsKey(resourceName)) {
				return idMap[resourceName];
			}
			else {
				Bitmap^ bmp = gcnew Bitmap(Image::FromFile(resourceName));
				BitmapData^ data = bmp->LockBits(Drawing::Rectangle(0, 0, bmp->Width, bmp->Height), 
					ImageLockMode::ReadOnly, PixelFormat::Format32bppArgb);
				
				size_t bufferSize = data->Stride * data->Height * 4;
				unsigned char* buffer = (unsigned char*)malloc(bufferSize);
				memset(buffer, 0, bufferSize);

				for (int i = 0; i < bufferSize; i += 4) {
					buffer[i + 0] = (CastPtr<unsigned char*>(data->Scan0))[i + 2]; // R
					buffer[i + 1] = (CastPtr<unsigned char*>(data->Scan0))[i + 1]; // G
					buffer[i + 2] = (CastPtr<unsigned char*>(data->Scan0))[i + 0]; // B
					buffer[i + 3] = (CastPtr<unsigned char*>(data->Scan0))[i + 3]; // A
				}

				unsigned int texID;
				glGenTextures(1, &texID);
				glBindTexture(GL_TEXTURE_2D, texID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp->Width, bmp->Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, buffer);

				free(buffer);
				bmp->UnlockBits(data);
				delete bmp;

				return texID;
			}
		}
		catch (Exception^ ex) {
			throw gcnew Exception("Failed to load texture resource: " + resourceName, ex);
		}
	}
};