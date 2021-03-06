#ifndef _Struttura_H_
#define _Struttura_H_

#include "Constants.h"
#include "Exceptions.h"
#include <list>

using namespace std;

namespace Earthquake {

struct token_ {
	short int type;
	float x1;
	float x2;
	float y1;
	float y2;
};


/**
* Class definitions
*	Get and Set methods shouldn't be used, they are provided for debugging purpose
**/
class Apertura;
class Aperture;


/**
* Class representing the "Apertura" non terminal:
*	APERTURE 	-> APERTURA
*	APERTURA 	-> apertura <isAbove, Height(0.1,0.2),isCentered,isLarger(0.15,0.3)> architrave
**/
class Apertura {

public:

	token_ *apertura;
	token_ *architrave;

	/**
	* this method checks if the condition of the objects "apertura" and "architrave"
	* are respected
	* Property to check: 
	*		apertura <isAbove, Height(0.1,0.2),isCentered,isLarger(0.15,0.3)> architrave
	* @throws PropertyViolationException if the properties aren't satisfied
	**/
	void checkProperties() throw(PropertyViolationException) {
		isAbove();

		float height = architrave->y2 - architrave->y1;

		if(height < 0.1 || height > 0.2)
			throw new PropertyViolationException(string("Architrave's height isn't between 0.1 and 0.2"));

		isCentered();
		isLarger();
	}

	/**
	 * checks if "apertura" is not above "architrave"
	 * @throws PropertyViolationException if the property isn't satisfied
	 */
	void isAbove() throw(PropertyViolationException) {
		if(architrave->y1 < apertura->y2)
			throw new PropertyViolationException(string("Architrave isn't above the Apertura"));
	}

	/**
	 * checks if "apertura" is centered (respect to "architrave")
	 * @throws PropertyViolationException if the property isn't satisfied
	 */
	void isCentered() throw(PropertyViolationException) {
		float rightDiff = architrave->x2 - apertura->x2;
		float leftDiff = apertura->x1 - architrave->x1;

		if(leftDiff != rightDiff)
			throw new PropertyViolationException(string("Apertura not centered"));
	}

	/**
	 * checks if the "architrave" is larger than the "apertura"
	 * @throws PropertyViolationException if the property isn't satisfied
	 */
	void isLarger() throw(PropertyViolationException) {
		float archLength = architrave->x2 - architrave->x1;
		float apLength = apertura->x2 - apertura->x1;

		if(archLength < apLength)
			throw new PropertyViolationException(string("Apertura is larger than Architrave"));
	}

	Apertura(token_ *ap, token_ *ar) throw(PropertyViolationException, IllegalArgumentException) {
		
		if( (!ap || !ar) || ap->type != APERTURA_ || ar->type != ARCHITRAVE_)
			throw new IllegalArgumentException(string("bad arguments"));

		apertura = ap;
		architrave = ar;

		checkProperties();
	}


	~Apertura() {
		delete apertura;
		delete architrave;
	}


	void setApertura(token_ *ap) {
		apertura = ap;
	}

	void setArchitrave(token_ *ar) {
		architrave = ar;
	}

	token_ *getApertura() {
		return apertura;
	}

	token_ *getArchitrave() {
		return architrave;
	}
};


/**
* Class representing the "Aperture" non terminal:
*	APERTURE 	-> APERTURE' <not intersect> APERTURA
**/
class Aperture{

private:
	list<Apertura> a;

public:

	Aperture(list<Apertura> ap): a(ap){};

	~Aperture(){ delete &a; };

	list<Apertura> getList(){
		return a;
	}

};

/**
* Class representing the "Interpiani" grammar non terminal
*	INTERPIANI 	-> linea_piano <isUnder, Height(0.2,0.4)> CORDOLO
*	INTERPIANI 	-> linea_piano
**/
class Interpiano {

public:

	token_ *linea_piano;
	token_ *cordolo;

	/**
	* this method checks if the condition of the objects "linea_piano" and "cordolo"
	* are respected
	* Property to check: 
	*		linea_piano <isUnder, Height(0.2,0.4)> CORDOLO
	*		linea_piano (if cordolo is Null)
	* @throws PropertyViolationException if the properties aren't satisfied
	**/
	void checkProperties(token_* c) throw(PropertyViolationException) {

		//if there isn't a cordolo, there is nothing to check
		if(!c)
			return;

		isUnder();

		//cordolo's height
		float height = cordolo->y2 - cordolo->y1;
		if(height < 0.2 || height > 0.4)
			throw new PropertyViolationException(string("Cordolo's height out of range"));
	}

	/**
	 * checks that "cordolo" is under "linea_piano"
	 * @throws PropertyViolationException
	 */
	void isUnder() throw(PropertyViolationException) {
		if(cordolo->y2 > linea_piano->y1)
			throw new PropertyViolationException(string("Cordolo is not under the Linea_Piano"));
	}

	//no null-pointer is allowed: in the grammar an "interpiano" must always have those tokens
	Interpiano(token_ *lp, token_ *c) throw(IllegalArgumentException, PropertyViolationException) {
		if(!lp || lp->type != LINEA_PIANO_)
			throw(new IllegalArgumentException(string("wrong token type")));
		if(c && c->type != CORDOLO_)
			throw(new IllegalArgumentException(string("wrong token type")));

		linea_piano = lp;
		cordolo = c;

	}

	//to create the object with only linea_piano
	Interpiano(token_ *lp) {
		Interpiano(lp, 0);
	}


	~Interpiano() {
		delete linea_piano;
		delete cordolo;
	}


	token_ *getLineaPiano() {
		return linea_piano;
	}

	token_ *getCorridoio() {
		return cordolo;
	}

	void setLineaPiano(token_ *lp) throw(IllegalArgumentException) {
		if(lp->type != LINEA_PIANO_)
			throw new IllegalArgumentException(string("Bad token type"));

		linea_piano = lp;
	}

	void setCordolo(token_ *c) throw(IllegalArgumentException) {
		if(c->type != CORDOLO_)
			throw new IllegalArgumentException(string("Bad token type"));

		cordolo = c;
	}

};


/**
* Class representing the "piani" non terminal in the grammar
*	PIANI 		-> PIANI  <contains> INTERPIANI
*	PIANI 		-> parete
**/
class Piani {

public:
	
	list<Interpiano> interpiani;
	token_ *parete;

	/**
	* this method checks if the condition of the objects "interpiani" and "parete"
	* are respected
	* Property to check: 
	*		PIANI�  <contains> INTERPIANI
	*		parete (if interpiani is empty)
	* @throws PropertyViolationException if the properties aren't satisfied
	**/
	void checkProperties() throw(PropertyViolationException) {

		if(interpiani.empty())
			return;

		/* NOTE: the iterator must be used as a pointer to use the methods of the object
		 * it contains
		 */

		//to check if "piano" contains "interpiani" means to check
		//if every interpiano in the list is contained in the "parete"
		for(list<Interpiano>::const_iterator it = interpiani.begin(); it != interpiani.end(); ++it) {

			float x1 = it->linea_piano->x1;
			if(it->cordolo && x1 > it->cordolo->x1)
				x1 = it->cordolo->x1;

			float x2 = it->linea_piano->x2;
			if(it->cordolo && x2 < it->cordolo->x2)
				x2 = it->cordolo->x2;

			float y1 = it->linea_piano->y1;
			if(it->cordolo && y1 > it->cordolo->y1)
				y1 = it->cordolo->y1;

			float y2 = it->linea_piano->y2;
			if(it->cordolo && y2 < it->cordolo->y2)
				y2 = it->cordolo->y2;

			if(parete->x1 > x1 || parete->x2 < x2 || parete->y1 > y1 || parete->y2 < y2)
				throw new PropertyViolationException(string("Interpiano not contained in Parete"));
		}

	}


	Piani(list<Interpiano> i, token_ *p=0) throw(IllegalArgumentException, PropertyViolationException) : interpiani(i) {
		if(p && p->type != PARETE_)
			throw new IllegalArgumentException(string("wrong token type"));
		
		checkProperties();

		parete = p;
	}

	//to create the object without the interpiano list
	Piani(token_ *p) {
		list<Interpiano> l;
		Piani(l, p);
	}

	~Piani() {
		interpiani.~list();
		delete parete;
	}

	token_ *getParete() {
		return parete;
	}

	list<Interpiano> getInterpiani() {
		return interpiani;
	}

	void setParete(token_ *p) throw(IllegalArgumentException) {
		if(p->type != PARETE_)
			throw new IllegalArgumentException(string("Wrong token type"));

		parete = p;
	}

	void setInterpiani(list<Interpiano> i) {
		interpiani = i;
	}
};





/**
* Class that represents the start simbol of the grammar
*	STRUTTURA 	-> PIANI <contains, not_intersect> APERTURE
**/
class Struttura {

public:
	Piani p;
	list<Apertura> a;

	/**
	* this method checks if the condition of the objects "piani" and "aperture"
	* are respected
	* Property to check: 
	*		PIANI <contains, not_intersect> APERTURE
	* @throws PropertyViolationException if the properties aren't satisfied
	**/
	void checkProperties() throw(PropertyViolationException) {

		for(list<Apertura>::const_iterator it = a.begin(); it != a.end(); ++it) {
			//Apertura *curr = it;

			//for Apertura's properties...
			float x1 = it->architrave->x1;
			float x2 = it->architrave->x2;

			float y1 = it->apertura->y1;
			float y2 = it->architrave->y2;

			//check if the Apertura doesn't intersect the Piano's parete
			token_ * par = p.getParete();
			if(par->x1 > x1 || par->x2 < x2 || par->y1 > y1 || par->y2 < y2)
				throw new PropertyViolationException(string("Apertura not contained in a Piano"));

			//check if the Apertura doesn't intersect any of the Piano's Interpiani
			list<Interpiano> interpiani = p.getInterpiani();
			for(list<Interpiano>::const_iterator interpiano_it = interpiani.begin(); interpiano_it != interpiani.end(); ++interpiano_it) {

				//it's useless to check x1 and x2
				float i_y2 = interpiano_it->linea_piano->y2;

				if(y1 < i_y2)
					throw new PropertyViolationException(string("Apertura interstects with an Interpiano"));
			}
		}

	}

	Struttura(Piani listaPiani, list<Apertura> listaAperture) : p(listaPiani), a(listaAperture) {
	}

	~Struttura() {
		p.~Piani();
		a.~list();
	}


	Piani getPiani() {
		return p;
	}

	list<Apertura> getAperture() {
		return a;
	}

	void setPiani(Piani &piani) {
		p = piani;
	}

	void setAperture(list<Apertura> a) {
		this->a = a;
	}

	/**
	* returns the list of Maschi calculated using "a" and "p"
	**/
	list<token_*> calcolaMaschi(){

	}

	/**
	* throws an exception if one or more Maschi doesn't respect their properties
	**/
	void verificaProprietaMaschio(list<token_*> l) throw(PropertyViolationException){

	}

};

}

#endif
