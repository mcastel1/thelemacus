#ifndef length_h
#define length_h

//read from file the content after 'name = ' and writes it into the *object (the element of class C).
// This works for any class C. On WIN32, if mode = 'RW' then filename is filepath  such as "C:/a/b.txt", while if mode = 'R' then filename is a resource name such as my_resource_file.
template<class C> void read_from_file(C* object, String name, String filename, [[maybe_unused]] String mode, [[maybe_unused]] String prefix) {

#ifdef __APPLE__
    //I am on APPLE operating system->the file is located in a folder in the .app package and I read it from there

    FileRW file;

    file.set_name(filename);
    file.open(String("in"), prefix);

    object->template read_from_stream<fstream>(name, file.value, true, prefix);

    file.close(prefix);

#endif

#ifdef _WIN32
    //I am on WIN32 operating system->I create an istringstream that containts the data of file (which is incorporated in the .exe file as a WIN32 resource),  and I read it from there


    if (mode == String("RW")) {
        //I am reading form a FileRW and filename is a file path

        FileRW file;

        file.set_name(filename);
        file.open(String("in"), prefix);

        object->template read_from_stream<fstream>(name, file.value, true, prefix);

        file.close(prefix);

    }
    else {

        if (mode == String("R")) {
            //I am reading from a FileR  and filename is a resource id

            FileR file;

            file.set_name(filename);
            file.open(prefix);

            object->template read_from_stream<istringstream>(name, file.value, true, prefix);

            file.close(prefix);

        }
        else {

            cout << prefix.value << RED << "Cannor read from file beacuse mode is invalid!\n" << RESET;

        }

    }


#endif

}

#endif
