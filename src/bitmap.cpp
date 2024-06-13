//
//  bitmap.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "bitmap.h"



//constructs a Bitmap object by setting no arguments
Bitmap::Bitmap() : wxBitmap() {

}

//constructs a Bitmap object by loading it from path path and rescaling it to size, by keeping its proportions
Bitmap::Bitmap(String path, wxSize size) :

#ifdef __APPLE__
    //I am on apple operating system->I set the bitmap from the file path

    wxBitmap(RescaleProportionally(wxBitmap(path.value, wxBITMAP_TYPE_PNG).ConvertToImage(), size)) {}

#endif

#ifdef _WIN32
//I am on windows operating system-> I load the bitmap from the windows resources

wxBitmap(wxBitmap(wxString(path.filename_without_folder_nor_extension(String("")).value), wxBITMAP_TYPE_PNG_RESOURCE).ConvertToImage().Scale(size.GetWidth(), size.GetHeight())) {}

#endif
