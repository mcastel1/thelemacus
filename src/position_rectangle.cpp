//
//  position_rectangle.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "position_rectangle.h"




PositionRectangle::PositionRectangle(void) {
    
    p_NW = new Position;
    p_SE = new Position;

}


//constructor which constructs p_NW and p_SE from p_NW_in and p_SE_in. For this to work, p_NW_in must lie at the NW of p_SE_in
PositionRectangle::PositionRectangle(Position p_NW_in, Position p_SE_in, [[maybe_unused]] String prefix) {

    p_NW = new Position;
    p_SE = new Position;
    
    (*p_NW) = p_NW_in;
    (*p_SE) = p_SE_in;

}

//set the content (not the memory adress) of *this equal to the content of x
void PositionRectangle::set(const PositionRectangle& x){
    
    p_NW->set((*(x.p_NW)));
    p_SE->set((*(x.p_SE)));
    
}



//returns true/false if p is containted in *this
bool PositionRectangle::Contains(Position p) {

    bool check_lambda;

    if ((p_NW->lambda) > (p_SE->lambda)) {
        //'normal' configuration where *this does not encompass the anti-greenwich meridian

        check_lambda = (((p.lambda) < (p_NW->lambda)) && ((p.lambda) > (p_SE->lambda)));

    }
    else {
        //'non-normal' configuration where *this  encompasses the anti-greenwich meridian

        check_lambda = (((p.lambda) < (p_NW->lambda)) || ((p.lambda) > (p_SE->lambda)));

    }

    return(check_lambda &&
           (((p.phi).normalize_pm_pi_ret() < ((p_NW->phi).normalize_pm_pi_ret())) &&
            ((p.phi).normalize_pm_pi_ret() > ((p_SE->phi).normalize_pm_pi_ret()))));

}


//if p !=NULL , compute rectangle of *this in units of x * y of the Mercator projection and writes it into *p and returns true, otherwise return false
bool PositionRectangle::SizeMercator(PositionProjection* p){
        
    if(p){
        
        Position p_NE, p_SW;
        PositionProjection q_A, q_B;

        p_NE = Position(p_SE->lambda, p_NW->phi);
        p_SW = Position(p_NW->lambda, p_SE->phi);
        q_A.NormalizeAndSetMercator(p_NE);
        q_B.NormalizeAndSetMercator(p_SW);
        
        (*p) = q_A-q_B;
    
        return true;
        
    }else{
        
        return false;
    }
    
}

