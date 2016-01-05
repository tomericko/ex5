/*
 * MoviesSystem.cpp
 *
 *  Created on: Nov 12, 2015
 *      Author: roi
 */

#include "MoviesSystem.h"

MoviesSystem* MoviesSystem::instance;
bool MoviesSystem::isConstruct;
pthread_mutex_t MoviesSystem::lock;
/*******************************************************************************
 * function name : ~MoviesSystem										       *
 * input : nothing														       *
 * output : nothing.													       *
 * explanation : destructor of a MoviesSystem.								   *
 *******************************************************************************/
MoviesSystem::~MoviesSystem() {
	for (vector<Movie*>::iterator it = this->movies.begin();
			it != this->movies.end(); ++it) {
		delete (*it);
	}
	for (vector<Professional*>::iterator it = this->professionals.begin();
			it != this->professionals.end(); ++it) {
		delete (*it);
	}
	for (vector<Type*>::iterator it = this->types.begin();
			it != this->types.end(); ++it) {
		delete (*it);
	}
}



/*******************************************************************************
 * function name : MoviesSystem											       *
 * input : nothing.														       *
 * output : nothing.													       *
 * explanation : constructor of moviesSystem object. creating vector of	       *
 *				movies and vector of professionals.						       *
 *******************************************************************************/

MoviesSystem::MoviesSystem() {
	this->movies = vector<Movie*>();
	this->professionals = vector<Professional*>();
	this->types = vector<Type*>();
	this->inUse = false;

	isConstruct = false;
	//this->server = server;
}

MoviesSystem* MoviesSystem::getInstance(){
	if(!isConstruct){

		pthread_mutex_lock(&lock);
		if(!isConstruct){
			MoviesSystem::instance = new MoviesSystem();
			isConstruct = true;
		}
		pthread_mutex_unlock(&lock);

	}

	return instance;
}

bool MoviesSystem::occupy(){
	if(!this->inUse){
		this->inUse = true;
		return true;
	}
	return false;
}

void MoviesSystem::setServer(Server* serv){
		this->server = serv;
	}
/*******************************************************************************
 * function name : start												       *
 * input : nothing.														       *
 * output : nothing.													       *
 * explanation : starting the operation of the movies system		  		   *
 *				(starting the manu).									       *
 *******************************************************************************/

void* MoviesSystem::start(void* var) {
	int* answer= new int(0);
	do {
		*answer = MoviesSystem::getInstance()->getCommand();
		sleep(1);
	} while (*answer);

	return answer;
}
/*******************************************************************************
 * function name : getCommand											       *
 * input : nothing.														       *
 * output : nothing.														   *
 * explanation : getting and operating the next command					       *
 *******************************************************************************/
int MoviesSystem::getCommand() {

	this->server->dataReceiver();
	string data = this->server->getDataReceived();
	vector<string> dat = this->split(data," ");

	int key = atoi(dat.at(0).c_str());

	switch (key) {
	case 1: {
		string id, name, summary = "";
		int length, year;
		float rank;
		id = dat.at(1);
		name = dat.at(2);
		length = atoi(dat.at(3).c_str());
		year = atoi(dat.at(4).c_str());
		rank = atof(dat.at(5).c_str());
		int size = dat.size();
		for(int i = 6; i < size; ++i) {
			summary += dat.at(i);
			if(i<size-1){
				summary += " ";
			}
		}
		this->setNewMovie(id, name, length, year, rank, summary);
	}
		break;
	case 2: {
		string specificDesc, gender, name;
		int id, professionalType, age;
		id = atoi(dat.at(2).c_str());
		professionalType = atoi(dat.at(1).c_str());
		age = atoi(dat.at(3).c_str());
		specificDesc = dat.at(4);
		gender = dat.at(5);
		int nameSize = dat.size();
		for(int i = 6; i < nameSize; ++i) {
			name += dat.at(i);
			if(i != nameSize-1){
				name += " ";
			}
		}
		this->setNewProfessional(professionalType, id, age, specificDesc,
				gender, name);
	}
		break;
	case 3: {
		string movieId;
		int professionalId;
		movieId = dat.at(1);
		professionalId = atoi(dat.at(2).c_str());
		this->addProfessionalToMovie(professionalId, movieId);
	}
		break;
	case 4: {
		string movieId, typeStr;
		movieId = dat.at(1);
		typeStr = dat.at(2);
		this->addTypeToMovie(movieId, typeStr);
	}
		break;
	case 5: {
		string movieId;
		int sortingType;
		movieId = dat.at(1);
		sortingType = atoi(dat.at(2).c_str());
		this->setSortingTypeMovie(movieId, sortingType);
	}
		break;
	case 6: {
		string movieId;
		movieId = dat.at(1);
		this->printAllProfessionalsOfMovie(movieId);
	}
		break;
	case 7: {
		string movieId;
		movieId = dat.at(1);
		this->printAllMovieDetails(movieId);
	}
		break;
	case 8: {
		string line = dat.at(1);
		vector<string> moviesId = this->split(line, ",");
		this->mergeMovies(moviesId);
	}
		break;
	case 9: {
		int professionalId;
		professionalId = atoi(dat.at(1).c_str());
		this->printAllMoviesOfProfessional(professionalId);
	}
		break;
	case 10: {
		string movieId;
		movieId = dat.at(1);
		this->deleteMovie(movieId);
	}
		break;
	case 11: {
		int professionalId;
		professionalId = atoi(dat.at(1).c_str());
		this->deleteProfessional(professionalId);
	}
		break;
	case 12: {
		string movieId;
		int professionalId;
		movieId = dat.at(1);
		professionalId = atoi(dat.at(2).c_str());
		this->removeProfessionalFromMovie(professionalId, movieId);
	}
		break;
	case 13: {
		this->printAllMovies();
	}
		break;
	case 14: {
		this->printAllProfessionals();
	}
		break;
	case 15: {
		string typeStr;
		typeStr = dat.at(1);
		this->printAllMoviesOfType(typeStr);
	}
		break;
	default:
		this->server->sendData("-1");
		return 0;
		break;
	}
	return 1;
}

/*******************************************************************************
 * function name : setNewMovie											       *
 * input : all movie parameters: code, length, year and rank as int,	   	   *
 *	  types as vector of string and summary as string.					       *
 * output : nothing.														   *
 * explanation : setting new movie object using Movie constructor.		       *
 *******************************************************************************/

void MoviesSystem::setNewMovie(string id, string name, int length, int year,
		float rank, string summary) {
	string result = "";
	if ((this->findMovie(id) == NULL)
			&& (this->isMovieInputValid(length, year, rank))) {
		Movie* movieToAdd = new Movie(id, name, length, year, rank, summary);
		this->movies.push_back(movieToAdd);
		result += "Success\n";
	} else {
		result += "Failure\n";
	}
	this->server->sendData(result);
}

/*******************************************************************************
 * function name : setNewProfessional									       *
 * input : all the professional parameters: id, age and gender as int,	       *
 *	  name as string.													       *
 * output : nothing.													       *
 * explanation : set new professional object using Professional constructor.   *
 *************************m******************************************************/

void MoviesSystem::setNewProfessional(int professionalType, int id, int age,
		string specificDesc, string gender, string name) {
	string result = "";
	Professional* professionalToAdd;
	if ((this->findProfessional(id) == NULL)
			&& (this->isProfessionalInputValid(professionalType, id, age,
					gender))) {
		switch (professionalType) {
		case 0: {
			Director* director = new Director(id, age, specificDesc, gender,
					name);
			professionalToAdd = (Professional*) (director);
		}
			break;
		case 1: {
			Actor* actor = new Actor(id, age, specificDesc, gender, name);
			professionalToAdd = (Professional*) (actor);
		}
			break;
		case 2: {
			ScreenWriter* screenWriter = new ScreenWriter(id, age, specificDesc,
					gender, name);
			professionalToAdd = (Professional*) (screenWriter);
		}
			break;
		case 3: {
			Producer* producer = new Producer(id, age, specificDesc, gender,
					name);
			professionalToAdd = (Professional*) (producer);
		}
			break;
		default:
			break;
		}
		this->professionals.push_back(professionalToAdd);
		result += "Success\n";
	} else {
		result += "Failure\n";
	}
	string resultToSend = result.c_str();
	this->server->sendData(resultToSend);
}

/*******************************************************************************
 * function name : addProfessionalToMovie								       *
 * input : professional id and movie id as int.							       *
 * output : nothing.													       *
 * explanation : searching the professional by id and adding it to		       *
 *				the movie staff and the movie to the professional movie vector.*
 *******************************************************************************/

void MoviesSystem::addProfessionalToMovie(int professionalToAddId,
		string movieId) {
	string result = "";
	Movie* movie = this->findMovie(movieId);
	Professional* professional = this->findProfessional(professionalToAddId);
	if ((movie != NULL) && (professional != NULL)) {
		movie->addProfessional(professional);
		professional->addMovie(movie);
		result += "Success\n";
	} else {
		result += "Failure\n";
	}
	string resultToSend = result.c_str();
	this->server->sendData(resultToSend);
}

/*******************************************************************************
 * function name : addTypeToMovie										       *
 * input : movie type as string  and movie id as int.					       *
 * output : nothing.													       *
 * explanation : adding type of movie to it's types vector.				       *
 *******************************************************************************/

void MoviesSystem::addTypeToMovie(string movieId, string typeToAddStr) {
	string result = "";
	Movie* movie = this->findMovie(movieId);
	Type* type = this->findType(typeToAddStr);
	if (movie != NULL) {
		if (type == NULL) {
			type = new Type(typeToAddStr);
			this->types.push_back(type);
		}
		movie->addType(type);
		result += "Success\n";
	} else {
		result += "Failure\n";
	}
	string resultToSend = result.c_str();
	this->server->sendData(resultToSend);
}

/*******************************************************************************
 * function name : setSortingTypeMovie									       *
 * input : sorting type as int.											       *
 * output : nothing.													       *
 * explanation : setting the sorting type of a movie.					       *
 *******************************************************************************/

void MoviesSystem::setSortingTypeMovie(string movieId, int sortingTypeToAdd) {
	string result = "";
	if (this->isSortingTypeValid(sortingTypeToAdd)) {
		Movie* movie = this->findMovie(movieId);
		movie->setSortingType(sortingTypeToAdd);
		result += "Success\n";
	} else {
		result += "Failure\n";
	}
	string resultToSend = result.c_str();
	this->server->sendData(resultToSend);
}

/*******************************************************************************
 * function name : printAllProfessionalsOfMovie							       *
 * input : movie id as int.												       *
 * output : nothing.													       *
 * explanation : searching the movie by it's id and printing all the	       *
 *				professionals of the movie.								       *
 *******************************************************************************/

void MoviesSystem::printAllProfessionalsOfMovie(string movieId) {
	string result = "";
	Movie* movie = this->findMovie(movieId);
	if (movie != NULL) {
		for (vector<Professional*>::iterator it = movie->getStaff().begin();
				it != movie->getStaff().end(); ++it) {
			result += (*it)->toString();
		}
	} else {
		result += "Failure\n";
	}
	string resultToSend = result.c_str();
	this->server->sendData(resultToSend);
}

/*******************************************************************************
 * function name : printAllMovieDetails									       *
 * input : movie id as int.												       *
 * output : nothing.													       *
 * explanation : searching the movie by it's id and printing all the	       *
 *				the movie details.										       *
 *******************************************************************************/

void MoviesSystem::printAllMovieDetails(string movieId) {
	string result = "";
	Movie* movie = this->findMovie(movieId);
	if (movie != NULL) {
		result += movie->toString();
	} else {
		result += "Failure\n";
	}
	string resultToSend = result.c_str();
	this->server->sendData(resultToSend);
}

/*******************************************************************************
 * function name : mergeMovies											       *
 * input : vector of Movie objects.										       *
 * output : nothing.													       *
 * explanation : searching the movies by their id, merging all the movies to   *
 one movie and adding it to movies vector.								       *
 *******************************************************************************/

void MoviesSystem::mergeMovies(vector<string> moviesId) {
	string result = "";
	for (vector<string>::iterator it = moviesId.begin(); it != moviesId.end();
			) {
		if (this->findMovie((*it)) == NULL) {
			moviesId.erase(it);
		} else {
			++it;
		}
	}
	if (moviesId.size() > 1) {
		Movie tempMovie = *(this->findMovie(*(moviesId.begin())));
		for (vector<string>::iterator it = moviesId.begin() + 1;
				it != moviesId.end(); ++it) {
			tempMovie = tempMovie + (*(this->findMovie((*it))));
		}
		StaffComparator* comparator	= new StaffComparator(1);
		tempMovie.setComparator(comparator);
		Movie* newMovie = new Movie(tempMovie.getId(), tempMovie.getName(),
				tempMovie.getLength(), tempMovie.getYear(), tempMovie.getRank(),
				tempMovie.getSummary());
		newMovie->setStaff(tempMovie.getStaff());
		for (vector<Professional*>::iterator it = newMovie->getStaff().begin();
				it != newMovie->getStaff().end(); ++it) {
			(*it)->addMovie(newMovie);
		}
		newMovie->setTypes(tempMovie.getTypes());
		for (vector<Type*>::iterator it = newMovie->getTypes().begin();
				it != newMovie->getTypes().end(); ++it) {
			(*it)->addMovie(newMovie);
		}
		if ((newMovie != NULL)
				&& (this->findMovie((*newMovie).getId()) == NULL)) {
			this->movies.push_back(newMovie);
			result += "Success\n";
		} else {
			//delete newMovie;
			result += "Failure\n";
		}
	} else {
		result += "Failure\n";
	}
	string resultToSend = result.c_str();
	this->server->sendData(resultToSend);
}

/*******************************************************************************
 * function name : printAllMoviesOfProfessional							       *
 * input : professional id as string.									       *
 * output : nothing.													       *
 * explanation : searching the professional by it's id and printing all the	   *
 *				movies of the professional.									   *
 *																		       *
 *******************************************************************************/

void MoviesSystem::printAllMoviesOfProfessional(int professionalId) {
	string result = "";
	Professional* professional = this->findProfessional(professionalId);
	if (professional != NULL) {
		for (vector<Movie*>::iterator it = professional->getMovies().begin();
				it != professional->getMovies().end(); ++it) {
			result += (*it)->toString();
		}
	} else {
		result += "Failure\n";
	}
	string resultToSend = result.c_str();
	this->server->sendData(resultToSend);
}

/*******************************************************************************
 * function name : printAllMoviesOfType									       *
 * input : type as string.												       *
 * output : nothing.													       *
 * explanation : searching the type by it's id and printing all the			   *
 *				movies of the type.											   *
 *																		       *
 *******************************************************************************/

void MoviesSystem::printAllMoviesOfType(string typeStr) {
	string result = "";
	Type* type = this->findType(typeStr);
	if (type != NULL) {
		for (vector<Movie*>::iterator it = type->getMovies().begin();
				it != type->getMovies().end(); ++it) {
			result += (*it)->toString();
		}
	} else {
		result += "Failure\n";
	}
	string resultToSend = result.c_str();
	this->server->sendData(resultToSend);
}

/*******************************************************************************
 * function name : deleteType											       *
 * input : movie id as string.											       *
 * output : nothing.													       *
 * explanation : searching the type by it's id and remove it from the types	   *
 *				vector and from each types vector of the movie types vector.   *
 *******************************************************************************/

void MoviesSystem::deleteType(string typeStr) {
	string result = "";
	if (this->findType(typeStr) != NULL) {
		for (vector<Type*>::iterator it = this->types.begin();
				it != this->types.end();) {
			if ((*it)->getType() == typeStr) {
				for (vector<Movie*>::iterator it2 = (*it)->getMovies().begin();
						it2 != (*it)->getMovies().end(); ++it2) {
					(*it2)->removeType(typeStr);
				}
				this->types.erase(it);
			} else {
				++it;
			}
		}
		result += "Success\n";
	} else {
		result += "Failure\n";
	}
	string resultToSend = result.c_str();
	this->server->sendData(resultToSend);
}

/*******************************************************************************
 * function name : deleteMovie											       *
 * input : movie id as int.												       *
 * output : nothing.													       *
 * explanation : searching the movie by it's id and remove it from the movies  *
 *				vector and from each movies vector of the movie staff vector.  *
 *******************************************************************************/

void MoviesSystem::deleteMovie(string movieId) {
	string result = "";
	if (this->findMovie(movieId) != NULL) {
		for (vector<Movie*>::iterator it = this->movies.begin();
				it != this->movies.end();) {
			if ((*it)->getId() == movieId) {
				for (vector<Professional*>::iterator it2 =
						(*it)->getStaff().begin();
						it2 != (*it)->getStaff().end(); ++it2) {
					(*it2)->removeMovie(movieId);
				}
				for (vector<Type*>::iterator it3 = (*it)->getTypes().begin();
						it3 != (*it)->getTypes().end(); ++it3) {
					(*it3)->removeMovie(movieId);
				}
				this->movies.erase(it);
			} else {
				++it;
			}
		}
		result += "Success\n";
	} else {
		result += "Failure\n";
	}
	string resultToSend = result.c_str();
	this->server->sendData(resultToSend);
}

/*******************************************************************************
 * function name : deleteProfessional									       *
 * input : professional id as int.										       *
 * output : nothing.													       *
 * explanation : searching the professional by it's id and remove it from the  *
 *				professional vector and from each staff vector of the	       *
 *				professional movies vector.								       *
 *******************************************************************************/

void MoviesSystem::deleteProfessional(int professionalId) {
	string result = "";
	if (this->findProfessional(professionalId) != NULL) {
		for (vector<Professional*>::iterator it = this->professionals.begin();
				it != this->professionals.end();) {
			if ((*it)->getId() == professionalId) {
				for (vector<Movie*>::iterator it2 = (*it)->getMovies().begin();
						it2 != (*it)->getMovies().end();) {
					if ((*it2)->findProfessional(professionalId) != NULL) {
						(*it2)->removeProfessional(professionalId);
						(*it)->removeMovie((*it2)->getId());
					} else {
						++it2;
					}
				}
				this->professionals.erase(it);
			} else {
				++it;
			}
		}
		result += "Success\n";
	} else {
		result += "Failure\n";
	}
	string resultToSend = result.c_str();
	this->server->sendData(resultToSend);
}

/*******************************************************************************
 * function name : removeProfessionalFromMovie							       *
 * input : professional id and movie id as int.							       *
 * output : nothing.													       *
 * explanation : searching the movie by it's id and remove the professional    *
 *				from the staff vector and removing the movie from the	       *
 *				professional movies vector.								       *
 *******************************************************************************/

void MoviesSystem::removeProfessionalFromMovie(int professionalId,
		string movieId) {
	string result = "";
	Movie* movie = this->findMovie(movieId);
	Professional* professional = this->findProfessional(professionalId);
	if ((movie != NULL) && (professional != NULL)
			&& (movie->findProfessional(professionalId) != NULL)) {
		movie->removeProfessional(professionalId);
		professional->removeMovie(movieId);
		result += "Success\n";
	} else {
		result += "Failure\n";
	}
	string resultToSend = result.c_str();
	this->server->sendData(resultToSend);
}

/*******************************************************************************
 * function name : printAllMovies										       *
 * input : nothing.														       *
 * output : nothing.													       *
 * explanation : printing all the details of each movie in the movies vector.  *
 *******************************************************************************/

void MoviesSystem::printAllMovies() {
	string result = "";
	if (this->movies.size() > 0) {
		for (vector<Movie*>::iterator it = this->movies.begin();
				it != this->movies.end(); ++it) {
			result += (*it)->toString();
		}
	}
	string resultToSend = result.c_str();
	this->server->sendData(resultToSend);
}

/*******************************************************************************
 * function name : printAllProfessionals								       *
 * input : nothing.														       *
 * output : nothing.													       *
 * explanation : printing all the professional details of each professional	   *
 *				in the professionals vector.							       *
 *******************************************************************************/

void MoviesSystem::printAllProfessionals() {
	string result = "";
	unsigned int size = this->professionals.size();
	if (size > 0) {
		for (vector<Professional*>::iterator it = this->professionals.begin();
				it != this->professionals.end(); ++it) {
			result += (*it)->toString();
		}
	}
	string resultToSend = result.c_str();
	this->server->sendData(resultToSend);
}

/*******************************************************************************
 * function name : getMovies											       *
 * input : nothing.														       *
 * output : vector of Movie objects.									       *
 * explanation : return the vector of all the movies of the movies system as   *
 * 				 vector	of Movie objects.								       *
 *******************************************************************************/
vector<Movie*>& MoviesSystem::getMovies() {
	return this->movies;
}

/*******************************************************************************
 * function name : getProfessionals										       *
 * input : nothing.														       *
 * output : vector of Professional objects.								       *
 * explanation : return the vector of all the professionals of the			   *
 * 				 movies system as vector of Professional objects.		       *
 *******************************************************************************/
vector<Professional*>& MoviesSystem::getProfessionals() {
	return this->professionals;
}

/*******************************************************************************
 * function name : getTypes												       *
 * input : nothing.														       *
 * output : vector of Type objects.										       *
 * explanation : return the vector of all the types of the					   *
 * 				 movies system as vector of types objects.				       *
 *******************************************************************************/
vector<Type*>& MoviesSystem::getTypes() {
	return this->types;
}

/*******************************************************************************
 * function name : findMovie											       *
 * input : movie id as int.												       *
 * output : movie with that id.											       *
 * explanation : finding professional with that id.						       *
 *******************************************************************************/

Movie* MoviesSystem::findMovie(string MovieId) {
	for (vector<Movie*>::iterator it = this->movies.begin();
			it != this->movies.end(); ++it) {
		if ((*it)->getId() == MovieId) {
			return (*it);
		}
	}
	return NULL;
}

/*******************************************************************************
 * function name : findProfessional										       *
 * input : professional id as int.										       *
 * output : professional with that id.									       *
 * explanation : finding professional with that id.						       *
 *******************************************************************************/
Professional* MoviesSystem::findProfessional(int ProfessionalId) {
	for (vector<Professional*>::iterator it = this->professionals.begin();
			it != this->professionals.end(); ++it) {
		if ((*it)->getId() == ProfessionalId) {
			return (*it);
		}
	}
	return NULL;
}

/*******************************************************************************
 * function name : findType												       *
 * input : Type as string.												       *
 * output : Type with that string.										       *
 * explanation : finding professional with that id.						       *
 *******************************************************************************/
Type* MoviesSystem::findType(string typeStr) {
	for (vector<Type*>::iterator it = this->types.begin();
			it != this->types.end(); ++it) {
		if ((*it)->getType() == typeStr) {
			return (*it);
		}
	}
	return NULL;
}

/*******************************************************************************
 * function name : split												       *
 * input : string and delimiters.										       *
 * output : vector of string after the split.							       *
 * explanation : split the string to the vector by the delimiters.		       *
 *******************************************************************************/
vector<string> MoviesSystem::split(string str, string delim) {
	string tmp;
	vector<string> result;
	int isDelim;
	for (string::iterator it = str.begin(); it <= str.end(); ++it) {
		isDelim = 0;
		for (string::iterator it2 = delim.begin(); it2 <= delim.end(); ++it2) {
			if (((const char) (*it) == (const char) (*it2))
					|| (it == str.end())) {
				isDelim = 1;
				break;
			}
		}
		if (!isDelim) {
			tmp += *it;
		} else if (tmp != "") {
			result.push_back(tmp);
			tmp = "";
		}
	}
	return result;
}

/*******************************************************************************
 * function name : isMovieInputValid									       *
 * input : length and year as int, rank as float.						       *
 * output : true if the input valid and false otherwise.				       *
 * explanation : check if the input of the movie is valid.				       *
 *******************************************************************************/
bool MoviesSystem::isMovieInputValid(int length, int year, float rank) {
	if ((rank <= 10) && (rank >= 0) && (year >= 0) && (year <= 2015)
			&& (length >= 0)) {
		return true;
	}
	return false;
}

/*******************************************************************************
 * function name : isProfessionalInputValid								       *
 * input : professional type, id and age  as int, gender as string.		       *
 * output : true if the input valid and false otherwise.				       *
 * explanation : check if the input of the professional is valid.		       *
 *******************************************************************************/
bool MoviesSystem::isProfessionalInputValid(int professionalType, int id,
		int age, string gender) {
	if ((professionalType >= 0) && (professionalType <= 3) && (id >= 0)
			&& (age >= 10) && ((gender == "male") || (gender == "female"))) {
		return true;
	}
	return false;
}

/*******************************************************************************
 * function name : isSortingTypeValid									       *
 * input : sortingType as int.											       *
 * output : true if the input valid and false otherwise.				       *
 * explanation : check if the input of the sortingType is valid.		       *
 *******************************************************************************/
bool MoviesSystem::isSortingTypeValid(int sortingType) {
	if ((sortingType >= 1) && (sortingType <= 3)) {
		return true;
	}
	return false;
}
