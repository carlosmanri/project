////////////////////////////////////////////////////////////////////////////////////////////////////////
//  --------------------------------------CoronaSphere 3D -----------------------------------------   //
//  Program:     Driver                                                                               //
//  Programmers: Carlos Manrique                                                                      //
//               Jose Manuel Estrada                                                                  //
//               Javier Diez                                                                          //
//               Victor Manuel Nunez                                                                  //
//               Lucia Mendez                                                                         //
//  Date:        8/10/14                                                                              //
//  Revision:    10/11/14                                                                             //
////////////////////////////////////////////////////////////////////////////////////////////////////////


#define pi 3,1415

float forces[3];
float torques[3];
float posicionInicial[12];
float posicionInicialEsferica[3];
float posicionInicialCartesiana[3];
float posicionPOI[3];
float posicionFinalEsferica[3];
float posicionFinal[3];
int PoiID;
float vector[3];
int counter;
float norm[3];
float posicionSatelite[12];

bool estamosalineados(){
    api.getMyZRState(posicionSatelite);
    game.getPOILoc(posicionPOI, PoiID);
    float dir[3];
    mathVecSubtract(dir,posicionPOI,posicionSatelite,3);
    mathVecNormalize(dir,3);
    float dotProd;
    dotProd = mathVecInner(dir,&posicionSatelite[6],3);
    if(dotProd>.985)
    {
       return true;
    }
    else{
       return false;
    }
}
void hacerfoto(){
  	float point[3];
  	calcularattitude();
  	point[0] = norm[0];
    point[1] = norm[1];
    point[2] = norm[2];
    if (estamosalineados()!= true){
        Movimiento();
        api.setAttitudeTarget(point);
    }
    DEBUG(("Esta bien alineado \n"));
    game.takePic(PoiID);
    DEBUG(("Llevas ocupadas ", game.getMemoryFilled(), " fotos de 2. \n"));
}
void calcularattitude(){
	float vect[3];
    game.getPOILoc(posicionPOI, PoiID);
	api.getMyZRState(posicionSatelite);
    mathVecSubtract(norm,posicionPOI,posicionSatelite,3);
    mathVecNormalize(norm,3);
    DEBUG(("Vector calculado.\n"));
}

void vaciar(){
    if (game.getMemoryFilled() == 2){
        DEBUG(("La memoria esta llena, procediendo a la subida de fotos.\n"));
        game.uploadPic();
        DEBUG(("Fotos subidas, espere 3 segundos para tener la camara operativa. \n"));
    }
    else{
        DEBUG(("Aun no has llenado la memoria, aprovechala mejor. \n"));
    }
}

void PosicionInicialEsfericasACartesianas(){
    posicionInicialCartesiana[0]=posicionInicialEsferica[0]*sinf(posicionInicialEsferica[1])*cosf(posicionInicialEsferica[2]);
    posicionInicialCartesiana[1]=posicionInicialEsferica[0]*sinf(posicionInicialEsferica[1])*sinf(posicionInicialEsferica[2]);
    posicionInicialCartesiana[2]=posicionInicialEsferica[0]*cosf(posicionInicialEsferica[1]);
    return;
}

void CalcularPuntoFinal(){
      	game.getPOILoc(posicionPOI, PoiID);
	if(posicionPOI[0]<0){
	    posicionFinal[0]=posicionPOI[0]-0.20;
	}
	if(posicionPOI[1]<0){
	    posicionFinal[1]=posicionPOI[1]-0.20;
	}
	if(posicionPOI[2]<0){
	    posicionFinal[2]=posicionPOI[2]-0.20;
	}
	if(posicionPOI[0]>0){
	    posicionFinal[0]=posicionPOI[0]+0.20;
	}
	if(posicionPOI[1]>0){
	    posicionFinal[1]=posicionPOI[1]+0.20;
	}
	if(posicionPOI[2]>0){
	    posicionFinal[2]=posicionPOI[2]+0.20;
	}
    DEBUG(("La posicion del poi es: %f ,%f ,%f \n", posicionPOI[0], posicionPOI[1], posicionPOI[2]));
    DEBUG(("La identidad del POI es: %i \n", PoiID));
}

void CalcularVector(){
    vector[0] = posicionFinal[0]-posicionInicial[0];
    vector[1] = posicionFinal[1]-posicionInicial[1];
    vector[2] = posicionFinal[2]-posicionInicial[2];
    DEBUG(("Los vectores son: %f ,%f ,%f \n", vector[0], vector[1], vector[2]));
    return;
}

void Movimiento(){
    
    float termino1 = 0;
    float termino2 = 0;
    termino1 = mathSquare(vector[1]/vector[0]);
    termino2 = mathSquare(vector[2]/vector[0]);
    float A = 1+termino1+termino2;
    
    float B = ((2/vector[0])*(vector[1]*(posicionInicial[1]-(vector[1]*posicionInicial[0]/vector[0]))
              +vector[2]*(posicionInicial[2]-(vector[2]*posicionInicial[0]/vector[0]))));
              
    float termino3 = 0;
    float termino4 = 0;
    termino3 = mathSquare(posicionInicial[1]-(vector[1]*posicionInicial[0]/vector[0]));
    termino4 = mathSquare(posicionInicial[2]-(vector[2]*posicionInicial[0]/vector[0]));
    float C = termino3+termino4 - 0.22*0.22;
    
    DEBUG(("A es: %f , B es: %f , C es: %f \n", A, B, C));
    
    if((B*B)<(4*A*C)){
        MovimientoRecto();
        DEBUG(("Movimiento recto \n"));
    }
    else {
        MovimientoEsferico();
        DEBUG(("Movimiento esferico \n"));
    }
    return;
}
void MovimientoEsferico(){
	posicionInicialCartesiana[0] = posicionInicial[0];
	posicionInicialCartesiana[1] = posicionInicial[1];
	posicionInicialCartesiana[2] = posicionInicial[2];
	
	int i = 0;
	float posicionSatelite[12];
    api.getMyZRState(posicionSatelite);
    
    PosicionInicialCartesianasAEsfericas();
        
    float MovimientoEsferico[3];
    MovimientoEsferico[0]=posicionInicialEsferica[0]*sinf(posicionInicialEsferica[1])*cosf(posicionInicialEsferica[2]);
    MovimientoEsferico[1]=posicionInicialEsferica[0]*sinf(posicionInicialEsferica[1])*sinf(posicionInicialEsferica[2]);
    MovimientoEsferico[2]=posicionInicialEsferica[0]*cosf(posicionInicialEsferica[1]);
    float diferencia = 0;
    float resultadoDivision = 0;
    
    //Movimiento Fi
    if(MovimientoEsferico[2]>posicionFinal[2]){
        diferencia = MovimientoEsferico[2]-posicionFinal[2];
        resultadoDivision = diferencia/6;
        
		for(i=0;i<6;i++){
			posicionInicialEsferica[2] = posicionInicialEsferica[2]-resultadoDivision;
			PosicionInicialEsfericasACartesianas();
			api.setPositionTarget(posicionInicialCartesiana);        
			while (!((posicionSatelite[0]<(posicionInicialCartesiana[0]+0.01) || posicionSatelite[0]>(posicionInicialCartesiana[0]-0.01))&&
					 (posicionSatelite[1]<(posicionInicialCartesiana[1]+0.01) || posicionSatelite[1]>(posicionInicialCartesiana[1]-0.01))&&
				     (posicionSatelite[2]<(posicionInicialCartesiana[2]+0.01) || posicionSatelite[2]>(posicionInicialCartesiana[2]-0.01)))){
				api.getMyZRState(posicionSatelite);
				api.setPositionTarget(posicionInicialCartesiana);
			}	
	    }
    }
    else if(MovimientoEsferico[2]<posicionFinal[2]){
        diferencia = MovimientoEsferico[2]-posicionFinal[2];
        resultadoDivision = diferencia/6;
        
		for(i=0;i<6;i++){
			posicionInicialEsferica[2] = posicionInicialEsferica[2]+resultadoDivision;
			PosicionInicialEsfericasACartesianas();
			api.setPositionTarget(posicionInicialCartesiana);        
			while (!((posicionSatelite[0]<(posicionInicialCartesiana[0]+0.01) || posicionSatelite[0]>(posicionInicialCartesiana[0]-0.01))&&
					 (posicionSatelite[1]<(posicionInicialCartesiana[1]+0.01) || posicionSatelite[1]>(posicionInicialCartesiana[1]-0.01))&&
					 (posicionSatelite[2]<(posicionInicialCartesiana[2]+0.01) || posicionSatelite[2]>(posicionInicialCartesiana[2]-0.01)))){
				api.getMyZRState(posicionSatelite);
				api.setPositionTarget(posicionInicialCartesiana);
			}	
	    }
    }

    //Movimiento Theta
        if(MovimientoEsferico[1]>posicionFinal[1]){
        diferencia = MovimientoEsferico[1]-posicionFinal[1];
        resultadoDivision = diferencia/6;
        
		for(i=0;i<6;i++){
			posicionInicialEsferica[1] = posicionInicialEsferica[1]-resultadoDivision;
			PosicionInicialEsfericasACartesianas();
			api.setPositionTarget(posicionInicialCartesiana);        
			while (!((posicionSatelite[0]<(posicionInicialCartesiana[0]+0.01) || posicionSatelite[0]>(posicionInicialCartesiana[0]-0.01))&&
					 (posicionSatelite[1]<(posicionInicialCartesiana[1]+0.01) || posicionSatelite[1]>(posicionInicialCartesiana[1]-0.01))&&
				     (posicionSatelite[2]<(posicionInicialCartesiana[2]+0.01) || posicionSatelite[2]>(posicionInicialCartesiana[2]-0.01)))){
				api.getMyZRState(posicionSatelite);
				api.setPositionTarget(posicionInicialCartesiana);
			}	
	    }
    }
    else if(MovimientoEsferico[1]<posicionFinal[1]){
        diferencia = MovimientoEsferico[1]-posicionFinal[1];
        resultadoDivision = diferencia/6;
        
		for(i=0;i<6;i++){
			posicionInicialEsferica[1] = posicionInicialEsferica[1]+resultadoDivision;
			PosicionInicialEsfericasACartesianas();
			api.setPositionTarget(posicionInicialCartesiana);        
			while (!((posicionSatelite[0]<(posicionInicialCartesiana[0]+0.01) || posicionSatelite[0]>(posicionInicialCartesiana[0]-0.01))&&
					 (posicionSatelite[1]<(posicionInicialCartesiana[1]+0.01) || posicionSatelite[1]>(posicionInicialCartesiana[1]-0.01))&&
					 (posicionSatelite[2]<(posicionInicialCartesiana[2]+0.01) || posicionSatelite[2]>(posicionInicialCartesiana[2]-0.01)))){
				api.getMyZRState(posicionSatelite);
				api.setPositionTarget(posicionInicialCartesiana);
			}	
	    }
    }
    //Para asegurar la posicion ahora nos movemos a posicionFinal[]//
    
    api.setPositionTarget(posicionFinal);
}


void MovimientoRecto(){
    api.setPositionTarget(posicionFinal);
}

void PosicionFinalCartesianasAEsfericas(){
   
    //pasar x a r
    posicionFinalEsferica[0] = sqrtf(posicionPOI[0]*posicionPOI[0]+posicionPOI[1]*posicionPOI[1]+posicionPOI[2]*posicionPOI[2]);

    //pasar y a theta
    if(posicionPOI[2]>0){
        posicionFinalEsferica[1] = atanf(sqrtf(posicionPOI[0]*posicionPOI[0]+posicionPOI[1]*posicionPOI[1])/posicionPOI[2]);
    }
    else if(posicionPOI[2]==0){
        posicionFinalEsferica[1] = (pi/2);
    }
    else if(posicionPOI[2]<0){
        posicionFinalEsferica[1] = pi+atanf(sqrtf(posicionPOI[0]*posicionPOI[0]+posicionPOI[1]*posicionPOI[1])/posicionPOI[2]);
    }
    //pasar z a fi
    if(posicionPOI[0]>0 && posicionPOI[1]>0){
        posicionFinalEsferica[2] = atanf(posicionPOI[1]/posicionPOI[0]);
    }
    else if(posicionPOI[0]>0 && posicionPOI[1]<0){
        posicionFinalEsferica[2] = 2*pi+atanf(posicionPOI[1]/posicionPOI[0]);    
    }
    else if(posicionPOI[0]==0){
        if(posicionPOI[1]<0){
            posicionFinalEsferica[2] = (pi*0.5)*1;
        }
        else if(posicionPOI[1]<0){
            posicionFinalEsferica[2] = (pi*0.5)*-1;
        }
    }
    else if(posicionPOI[0]<0){
        posicionFinalEsferica[2] = pi+atanf(posicionPOI[1]/posicionPOI[0]);
    }
    //ahora la posicion del poi esta en esfericas
    // el siguiente paso es sumarle al radio lo que le falta para estar en la zona outer
    float DiferenciaEntreRyOuter = 0;
    DiferenciaEntreRyOuter = 0.5-posicionFinalEsferica[0];
    posicionFinalEsferica[0] = posicionFinalEsferica[0]+DiferenciaEntreRyOuter;
    DEBUG(("La posicion final esferica es: %f ,%f ,%f \n", posicionFinalEsferica[0], posicionFinalEsferica[1], posicionFinalEsferica[2]));
    return;
}
 
void PosicionFinalEsfericasACartesianas(){
    posicionFinal[0] = posicionFinalEsferica[0]*sinf(posicionFinalEsferica[1])*cosf(posicionFinalEsferica[2]);
    posicionFinal[1] = posicionFinalEsferica[0]*sinf(posicionFinalEsferica[1])*sinf(posicionFinalEsferica[2]);
    posicionFinal[2] = posicionFinalEsferica[0]*cosf(posicionFinalEsferica[1]);
    DEBUG(("La posicion final cartesiana es: %f ,%f ,%f \n", posicionFinal[0], posicionFinal[1], posicionFinal[2]));
    return;
}


void PosicionInicialCartesianasAEsfericas(){
    //pasar x
    posicionInicialEsferica[0] = sqrtf(posicionInicial[0]*posicionInicial[0]+posicionInicial[1]*posicionInicial[1]+posicionInicial[2]*posicionInicial[2]);;
    //pasar y
    if(posicionInicial[2]>0){
        posicionInicialEsferica[1] = atanf(sqrtf(posicionInicial[0]*posicionInicial[0]+posicionInicial[1]*posicionInicial[1])/posicionInicial[2]);
    }
    else if(posicionInicial[2]==0){
        posicionInicialEsferica[1] = pi/2;
    }
    else if(posicionInicial[2]<0){
        posicionInicialEsferica[1] = pi+atanf(sqrtf(posicionInicial[0]*posicionInicial[0]+posicionInicial[1]*posicionInicial[1])/posicionInicial[2]);
    }
    //pasar z
    if(posicionInicial[0]>0 && posicionInicial[1]>0){
        posicionInicialEsferica[2] = atanf(posicionInicial[1]/posicionInicial[0]);
    }
    else if(posicionInicial[0]>0 && posicionInicial[1]<0){
        posicionInicialEsferica[2] = 2*pi+atanf(posicionInicial[1]/posicionInicial[0]);    
    }
    else if(posicionInicial[0]==0){
        if(posicionInicial[1]<0){
            posicionInicialEsferica[2] = (pi*0.5)*1;
        }
        else if(posicionInicial[1]<0){
            posicionInicialEsferica[2] = (pi*0.5)*-1;
        }
    }
    else if(posicionInicial[0]<0){
        posicionInicialEsferica[2] = pi+atanf(posicionInicial[1]/posicionInicial[0]);
    }
    DEBUG(("La posicion inicial esferica es: %f ,%f ,%f \n", posicionInicialEsferica[0], posicionInicialEsferica[1], posicionInicialEsferica[2]));
    return;
}


void init(){
    counter = 0;
    PoiID = 0;
    vector[0] = 0;
    vector[1] = 0;
    vector[2] = 0;
    posicionInicial[0] = 0;
    posicionInicial[1] = 0;    
    posicionInicial[2] = 0;    
    posicionInicial[3] = 0;    
    posicionInicial[4] = 0;    
    posicionInicial[5] = 0;    
    posicionInicial[6] = 0;    
    posicionInicial[7] = 0;
    posicionInicial[8] = 0;    
    posicionInicial[9] = 0;    
    posicionInicial[10] = 0;    
    posicionInicial[11] = 0;
    posicionInicialEsferica[0] = 0;
    posicionInicialEsferica[1] = 0;    
    posicionInicialEsferica[2] = 0;
    posicionInicialCartesiana[0] = 0;
    posicionInicialCartesiana[1] = 0;    
    posicionInicialCartesiana[2] = 0; 
    posicionPOI[0] = 0;
    posicionPOI[1] = 0;
    posicionPOI[2] = 0;
    posicionFinalEsferica[0] = 0;
    posicionFinalEsferica[1] = 0;
    posicionFinalEsferica[2] = 0;
    posicionFinal[0] = 0;   
    posicionFinal[1] = 0;   
    posicionFinal[2] = 0;   
    forces[0] = 0;
    forces[1] = 0;
    forces[2] = 0;
    torques[0] = 0;
    torques[1] = 0;
    torques[2] = 0;
    
    api.setForces(forces);
    DEBUG(("Las forces son: %f ,%f ,%f \n", forces[0], forces[1], forces[2]));
    api.setTorques(torques);
    DEBUG(("Las torques son: %f ,%f ,%f \n", torques[0], forces[1], forces[2]));
    api.getMyZRState(posicionInicial);
    DEBUG(("La posicion inicial es: %f ,%f ,%f \n", posicionInicial[0], posicionInicial[1], posicionInicial[2]));
    

}

void loop(){
    vaciar();
    api.getMyZRState(posicionInicial);
    CalcularPuntoFinal();
    CalcularVector();
    Movimiento();
    hacerfoto();    
	counter++;
}

