//
//  bodyfield.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef bodyfield_h
#define bodyfield_h

//P is the type of the frame which hosts *this
template<class P> class BodyField : public MultipleItemField<P, Body, CheckBody<P>>{
    
public:

    Catalog* catalog;
    
    BodyField(wxPanel*, Body*, Catalog*, vector<int>*);
    void set(void);
    template<class T> void get(T&);
    template<class E> void OnEdit(E&);
    
};


#endif
