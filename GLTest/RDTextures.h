#pragma once

#include "pch.h"
#include <glad/glad.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

public ref class RDTextures abstract sealed {
private:
	static Dictionary<String^, int>^ idMap = gcnew Dictionary<String^, int>();

public:
	static unsigned int LoadTexture(String^ resourceName, int mode) {
		try {
			if (idMap->ContainsKey(resourceName)) {
				return idMap[resourceName];
			}
			else {
				Bitmap^ bmp = nullptr;
				
				try {
					bmp = gcnew Bitmap(Image::FromFile(resourceName));
				}
				catch (Exception^ ex) {
					Console::WriteLine("Failed to read texture {0}: {1}", resourceName, ex);
					bmp = gcnew Bitmap(256, 256);
					for (int x = 0; x < 256; x++) {
						for (int y = 0; y < 256; y++) {
							bmp->SetPixel(x, y, Color::Purple);
						}
					}
				}

				BitmapData^ data = bmp->LockBits(Drawing::Rectangle(0, 0, bmp->Width, bmp->Height), 
					ImageLockMode::ReadOnly, PixelFormat::Format32bppArgb);
				
#pragma warning(push)
#pragma warning(disable: 6011)
#pragma warning(disable: 6281)
#pragma warning(disable: 6386)

				size_t bufferSize = data->Width * data->Height * 4;
				unsigned char* buffer = (unsigned char*)calloc(bufferSize, 1);

				for (int i = 0; i < bufferSize; i += 4) {
					buffer[i + 0] = (CastPtr<unsigned char*>(data->Scan0))[i + 2]; // R
					buffer[i + 1] = (CastPtr<unsigned char*>(data->Scan0))[i + 1]; // G
					buffer[i + 2] = (CastPtr<unsigned char*>(data->Scan0))[i + 0]; // B
					buffer[i + 3] = (CastPtr<unsigned char*>(data->Scan0))[i + 3]; // A
				}

#pragma warning(pop)

				unsigned int texID;
				glGenTextures(1, &texID);
				glBindTexture(GL_TEXTURE_2D, texID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp->Width, bmp->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

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