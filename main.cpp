#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define MAXCHAR 1000

using namespace std ;

struct Donnee {
    string *str ;
    int entier ;
} ;

struct Colonne {
    string nom ;
    string type ;
    size_t nbValeursDifferentes ;
} ;

struct Tableau {
    string nom ;
    size_t nbLignes ;
    size_t nbColonnes ;
    Colonne *colonnes ;
} ;

struct FichierDonnees  {
    size_t nbTableaux ;
    Tableau *tableaux ;
} ;

struct Jointure {
    string opGauche ;
    string opDroit ;
} ;

struct Condition {
    string opGauche ;
    string opDroit ;
    string operateur ;
} ;

struct Requete {
    size_t nbColonnes ;
    size_t nbTableaux ;
    size_t nbJointures ;
    size_t nbConditions ;
    string *tableaux ;
    string *colonnes ;
    Jointure *jointures ;
    Condition *conditions ;
} ;

size_t getNbVirgules ( string line ) {
    size_t res = 0 ;
    for ( string::iterator it = line . begin () ; it != line . end () ; it++ ) {
        if ( *it == ',' ) {
            res++ ;
        }
    }
    return res ;
}

size_t getNbPoints ( string line ) {
    size_t res = 0 ;
    for ( string::iterator it = line . begin () ; it != line . end () ; it++ ) {
        if ( *it == '.' ) {
            res++ ;
        }
    }
    return res ;
}

size_t getNbOpComparaisons ( string line ) {
    size_t res = 0 ;
    for ( string::iterator it = line . begin () ; it != line . end () ; it++ ) {
        if ( *it == '<' || *it == '>' ) {
            res++ ;
        }
    }
    return res ;
}

size_t getNbSpaces ( string line ) {
    size_t res = 0 ;
    for ( string::iterator it = line . begin () ; it != line . end () ; it++ ) {
        if ( (int) *it < 33 || (int) *it > 126 ) {
            res++ ;
        }
    }
    return res ;
}

Tableau & getTabParNom ( const FichierDonnees &fichierD , const string & nomTableau ) {
    for ( size_t i = 0 ; i < fichierD . nbTableaux ; i++ ) {
        if ( fichierD . tableaux [ i ] . nom == nomTableau ) {
            return fichierD . tableaux [ i ] ;
        }
    }
    return fichierD . tableaux [ 0 ] ;
}

Colonne & getColonneParNom ( Tableau tableau , const string &nomColonne ) {
    for ( size_t i = 0 ; i < tableau . nbColonnes ; i++ ) {
        if ( tableau . colonnes [ i ] . nom == nomColonne ) {
            return tableau . colonnes [ i ] ;
        }
    }
    return tableau . colonnes [ 0 ] ;
}

string clearSpaces ( string line ) {
    char *res = new char [ line . size () - getNbSpaces ( line ) ] ;
    size_t indice = 0 ;
    for ( size_t i = 0 ; i < line . size () ; i++ ) {
        if ( (int) line [ i ] > 32 && (int) line [ i ] < 127 ) {
            res [ indice ] = line [ i ] ;
            indice++ ;
        }
    }
    string tmp = res ;
    tmp = tmp . substr ( 0 , line . size () - getNbSpaces ( line ) ) ;
    return tmp ;
}

bool isEntier ( string mot ) {
    for ( size_t i = 0 ; i < mot . size () ; i++ ) {
        if ( mot[i] != '0' && mot[i]!='1' && mot[i]!='2' && mot[i]!='3' && mot[i]!='4'
        && mot[i]!='5' && mot[i]!='6' && mot[i]!='7' && mot[i]!='8' && mot[i]!='9' ) {
            return false ;
        }
    }
    return true ;
}

int charToInt ( char c ) {
    return ( c - '0' ) ;
}

int stringToInt ( string mot ) {
    int res = 0 ;
    size_t coef = 1 ;
    for ( size_t i = 0 ; i < mot . size () ; i++ ) {
        res += charToInt ( mot [ mot . size () - 1 - i ] ) * coef ;
        coef *= 10 ;
    }
    return res ;
}


string charVectToString ( vector < char > mot ) {
    char* res = new char [ mot . size () ] ;
    for ( size_t i = 0 ; i < mot . size () ; i++ ) {
        res [ i ] = mot [ i ] ;
    }
    return res ;
}

void openFichier ( ifstream &is , string fichier ) {
    is . open ( fichier ) ;
    if ( ! is . is_open () ) {
        cout << "Impossible d'ouvrir le fichier: " << fichier << endl ;
    }
}

void nbTableaux ( ifstream &is , FichierDonnees &fichierD ) {
    fichierD . nbTableaux = 0 ;
    is . seekg ( is . beg ) ;
    string line ;
    while ( ! is . eof () ) {
        getline ( is , line ) ;
        if ( getNbVirgules ( line ) == 0 ) {
            ( fichierD . nbTableaux )++ ;
        }
    }
}

void nomsTableaux ( ifstream &is , FichierDonnees &fichierD ) {
    is . seekg ( is . beg ) ;
    string line ;
    size_t indice = 0 ;
    while ( ! is . eof () ) {
        getline ( is , line ) ;
        if ( getNbVirgules ( line ) == 0 ) {
            fichierD . tableaux [ indice ] . nom = line . substr ( 0 , line . size () - 1 ) ;
            indice++ ;
        }
    }
}

void nbsColonnes ( ifstream &is , FichierDonnees &fichierD ) {
    is . seekg ( is . beg ) ;
    string line ;
    size_t indice = 0 ;
    while ( ! is . eof () ) {
        getline ( is , line ) ;
        if ( getNbVirgules ( line ) == 0 ) {
            getline ( is , line ) ;
            fichierD . tableaux [ indice ] . nbColonnes = getNbVirgules ( line ) ;
            indice++ ;
        }
    }
}

void nbLignes ( ifstream &is , FichierDonnees &fichierD ) {
    is . seekg ( is . beg ) ;
    string line ;
    size_t indice = 0 ;
    getline ( is , line ) ;
    getline ( is , line ) ;
    while ( ! is . eof () ) {
        getline ( is , line ) ;
        if ( getNbVirgules ( line ) != 0 ) {
            fichierD . tableaux [ indice ] . nbLignes++ ;
        }
        else {
            getline ( is , line ) ;
            indice++ ;
        }
    }
}

void nomsColonnes ( ifstream &is , FichierDonnees &fichierD ) {
    is . seekg ( is . beg ) ;
    string line ;
    size_t indiceTableau = 0 ;
    size_t indiceColonne = 0 ;
    while ( ! is . eof () ) {
        getline ( is , line ) ;
        if ( getNbVirgules ( line ) == 0 ) {
            getline ( is , line ) ;
            for ( indiceColonne = 0 ; indiceColonne < fichierD . tableaux [ indiceTableau ] . nbColonnes ; indiceColonne++ ) {
                fichierD . tableaux [ indiceTableau ] . colonnes [ indiceColonne ] . nom = line . substr ( 0 , min ( line . find ( ',' ) , line . find ( ';' ) ) ) ;
                line = line . substr ( line . find ( ',' ) + 1 , line . find ( ';' ) ) ;
            }
            indiceTableau++ ;
        }
    }
}

void typesColonnes ( ifstream &is , FichierDonnees &fichierD ) {
    is . seekg ( is . beg ) ;
    string line ;
    size_t indiceTableau = 0 ;
    size_t indiceColonne = 0 ;
    while ( ! is . eof () ) {
        getline ( is , line ) ;
        if ( getNbVirgules ( line ) == 0 ) {
            getline ( is , line ) ;
            getline ( is , line ) ;
            for ( indiceColonne = 0 ; indiceColonne < fichierD . tableaux [ indiceTableau ] . nbColonnes ; indiceColonne++ ) {
                if ( isEntier ( line . substr ( 0 , line . find ( ',' ) ) ) ) {
                    fichierD . tableaux [ indiceTableau ] . colonnes [ indiceColonne ] . type = "entier" ;
                }
                else {
                    fichierD . tableaux [ indiceTableau ] . colonnes [ indiceColonne ] . type = "string" ;
                }
                line = line . substr ( line . find ( ',' ) + 1 , line . find ( ';' ) ) ;
            }
            indiceTableau++ ;
            indiceColonne = 0 ;
        }
    }
}

Donnee getValeur ( string nomTableau , string nomColonne , size_t numLigne , Donnee *donnees , const FichierDonnees &fichierD ) {
    size_t tabIndice = 0 ;
    size_t dataIndice = 0 ;
    size_t colonneIndice = 0 ;
    for ( size_t i = 0 ; i < fichierD . nbTableaux ; i++ ) {
        if ( fichierD . tableaux [ i ] . nom == nomTableau ) {
            tabIndice = i ;
            break ;
        }
    }
    for ( size_t i = 0 ; i < fichierD . tableaux [ tabIndice ] . nbColonnes ; i++ ) {
        if ( fichierD . tableaux [ tabIndice ] . colonnes [ i ] . nom == nomColonne ) {
            colonneIndice = i ;
            break ;
        }
    }
    for ( size_t i = 0 ; i < tabIndice ; i++ ) {
        dataIndice += ( fichierD . tableaux [ i ] . nbLignes * fichierD . tableaux [ i ] . nbColonnes ) ;
    }
    dataIndice += ( numLigne * fichierD . tableaux [ tabIndice ] . nbColonnes + colonneIndice ) ;
    return donnees [ dataIndice ] ;
}

void lectureValeures ( string *strings , Donnee* donnees , FichierDonnees &fichierD , ifstream &is ) {
    is . seekg ( is . beg ) ;
    string line ;
    string mot ;
    size_t donneesIndice = 0 ;
    size_t stringsIndice = 0 ;
    int indiceTableau = -1 ;
    while ( ! is . eof () ) {
        getline ( is , line ) ;
        if ( getNbVirgules ( line ) != 0 ) {
            for  ( size_t i = 0 ; i < fichierD . tableaux [ indiceTableau ] . nbColonnes ; i++ ) {
                mot = line . substr ( 0 , line . find ( ',' ) ) ;
                if ( isEntier ( mot ) ) {
                    donnees [ donneesIndice ] . entier = stringToInt ( mot ) ;
                }
                else {
                    strings [ stringsIndice ] = mot ;
                    donnees [ donneesIndice ] . str = & strings [ stringsIndice ]  ;
                }
                stringsIndice++ ;
                donneesIndice++ ;
                line = line . substr ( line . find ( ',' ) + 1 , line . find ( ';' ) ) ;
            }
        }
        else {
            indiceTableau++ ;
            getline ( is , line ) ;
        }
    }
}

void getNbValeursDifferents ( FichierDonnees &fichierD , Donnee *donnees , string *strings ) {
    for ( size_t i = 0 ; i < fichierD . nbTableaux ; i++ ) {
        for ( size_t j = 0 ; j < fichierD . tableaux [ i ] . nbColonnes ; j++ ) {
            fichierD . tableaux [ i ] . colonnes [ j ] . nbValeursDifferentes = 0 ;
            for ( size_t k = 0 ; k < fichierD . tableaux [ i ] . nbLignes ; k++ ) {
                bool tmp = false ;
                for ( size_t l = 0 ; l < k ; l++ ) {
                    if ( getValeur ( fichierD . tableaux [ i ] . nom ,
                    fichierD . tableaux [ i ] . colonnes [ j ] . nom , k , donnees , fichierD ) . entier
                    == getValeur ( fichierD . tableaux [ i ] . nom ,
                    fichierD . tableaux [ i ] . colonnes [ j ] . nom , l , donnees , fichierD ) . entier
                    && ( getValeur ( fichierD . tableaux [ i ] . nom ,
                    fichierD . tableaux [ i ] . colonnes [ j ] . nom , k , donnees , fichierD ) . str == 0 ||
                    *getValeur ( fichierD . tableaux [ i ] . nom ,
                    fichierD . tableaux [ i ] . colonnes [ j ] . nom , k , donnees , fichierD ) . str
                    == *getValeur ( fichierD . tableaux [ i ] . nom ,
                    fichierD . tableaux [ i ] . colonnes [ j ] . nom , l , donnees , fichierD ) . str ) ) {
                        tmp = true ;
                    }
                }
                if ( tmp == false ) {
                    fichierD . tableaux [ i ] . colonnes [ j ] . nbValeursDifferentes ++ ;
                }
            }
        }
    }
}

void jointeurParseur ( string jointure , string *tGauche , string *cGauche , string *tDroit , string *cDroit ) {
    string opGauche = jointure . substr ( 0 , jointure . find ( '=' ) ) ;
    string opDroit = jointure . substr ( jointure . find ( '=' ) + 1 , jointure . find ( '\0' ) ) ;
    *tGauche = opGauche . substr ( 0 , opGauche . find ( '.' ) ) ;
    *cGauche = opGauche . substr ( opGauche . find ( '.' ) + 1 , opGauche . find ( '\0' ) ) ;
    *tDroit = opDroit . substr ( 0 , opDroit . find ( '.' ) ) ;
    *cDroit = opDroit . substr ( opDroit . find ( '.' ) + 1 , opDroit . find ( '\0' ) ) ;
}

void conditionParseur ( string condition , string *opGauche , string *operateur , string *opDroit ) {
    size_t indiceEgalite = condition . find ( '=' ) ;
    size_t indiceSup = condition . find ( '>' ) ;
    size_t indiceInf = condition . find ( '<' ) ;
    size_t indiceComp = min ( min ( indiceEgalite , indiceSup ) , indiceInf ) ;
    if ( condition [ indiceComp ] == '=' ) {
        *operateur = "=" ;
    }
    else if ( condition [ indiceComp ] == '>' ) {
        if ( condition [ indiceComp + 1 ] == '=' ) {
            *operateur = ">=" ;
        }
        else {
            *operateur = ">" ;
        }
    }
    else if ( condition [ indiceComp ] == '<' ) {
        if ( condition [ indiceComp + 1 ] == '=' ) {
            *operateur = "<=" ;
        }
        else {
            *operateur = "<" ;
        }
    }
    *opGauche = condition . substr ( 0 , indiceComp ) ;
    *opDroit = condition . substr ( indiceComp + ( *operateur ) . size () , condition . size () ) ;
}

void requeteDimensions ( ifstream &is , Requete &fichierR ) {
    fichierR . nbJointures = 0 ;
    fichierR . nbConditions = 0 ;
    is . seekg ( is . beg ) ;
    string line ;
    getline ( is , line ) ;
    fichierR . nbColonnes = getNbVirgules ( line ) + 1 ;
    getline ( is , line ) ;
    fichierR . nbTableaux = getNbVirgules ( line ) + 1 ;
    while ( ! is . eof () ) {
        getline ( is , line ) ;
        if ( getNbPoints ( line ) == 2 && getNbOpComparaisons ( line ) == 0 ) {
            fichierR . nbJointures ++ ;
        }
        else {
            fichierR . nbConditions ++ ;
        }
    }
}

bool tableauExiste ( const FichierDonnees &fichierD , const string &tableaux ) {
    for ( size_t i = 0 ; i < fichierD . nbTableaux ; i++ ) {
        if ( fichierD . tableaux [ i ] . nom == tableaux ) {
            return true ;
        }
    }
    return false ;
}

bool colonneExiste ( const Tableau &tableau , const string &colonne ) {
    for ( size_t i = 0 ; i < tableau . nbColonnes ; i++ ) {
        if ( tableau . colonnes [ i ] . nom == colonne ) {
            return true ;
        }
    }
    return false ;
}

bool verifierJointure ( const FichierDonnees &fichierD , const string &tGauche , const string &cGauche , const string &tDroit , const string &cDroit ) {
    if ( ! tableauExiste ( fichierD , tGauche ) ) {
        cout << "Tableau \"" << tGauche << "\" n'existe pas." << endl ;
        return false ;
    }
    if ( ! tableauExiste ( fichierD , tDroit ) ) {
        cout << "Tableau \"" << tDroit << "\" n'existe pas." << endl ;
        return false ;
    }
    Tableau tGaucheT = getTabParNom ( fichierD , tGauche ) ;
    Tableau tDroitT = getTabParNom ( fichierD , tDroit ) ;
    if ( ! colonneExiste ( tGaucheT , cGauche ) ) {
        cout << "Colonne \"" << cGauche << "\" n'existe pas." << endl ;
        return false ;
    }
    if ( ! colonneExiste ( tDroitT , cDroit ) ) {
        cout << "Colonne \"" << cDroit << "\" n'existe pas." << endl ;
        return false ;
    }
    Colonne cGaucheC = getColonneParNom ( tGaucheT , cGauche ) ;
    Colonne cDroitC = getColonneParNom ( tDroitT , cDroit ) ;
    if ( cGaucheC . type != cDroitC . type ) {
        cout << "Type de colonnes \"" << cGauche << "\" et \"" << cDroit << "\" differents." << endl ;
        return false ;
    }
    return true ;
}

void lectureRequete ( ifstream &is , Requete &fichierR , FichierDonnees &fichierD ) {
    is . seekg ( is . beg ) ;
    string line ;
    string mot ;
    size_t indice = 0 ;
    getline ( is , line ) ;
    line = line . substr ( line . find ( "SELECT" ) + 6 , line . size () ) ;
    line = clearSpaces ( line ) ;
    while ( indice < fichierR . nbColonnes ) {
        mot = line . substr ( 0 , min ( line . find ( ',' ) , line . size () ) ) ;
        fichierR . colonnes [ indice ] = mot ;
        line = line . substr ( line . find ( ',' ) + 1 , line . size () - 1 ) ;
        indice++ ;
    }
    indice = 0 ;
    getline ( is , line ) ;
    line = line . substr ( line . find ( "FROM" ) + 4 , line . size () ) ;
    line = clearSpaces ( line ) ;
    while ( indice < fichierR . nbTableaux ) {
        mot = line . substr ( 0 , min ( line . find ( ',' ) , line . find ( '\0' ) ) ) ;
        fichierR . tableaux [ indice ] = mot ;
        line = line . substr ( line . find ( ',' ) + 1 , line . size () - 1 ) ;
        indice++ ;
    }
    size_t indiceJointure = 0 ;
    size_t indiceCondition = 0 ;
    string cGauche , tGauche , cDroit , tDroit ;
    string opGauche , operateur , opDroit ;
    while ( ! is . eof () ) {
        getline ( is , line ) ;
        line = line . substr ( line . find ( "WHERE" ) + 5 , line . size () ) ;
        line = clearSpaces ( line ) ;
        if ( getNbPoints ( line ) == 2 && getNbOpComparaisons ( line ) == 0 ) {
            jointeurParseur ( line , &tGauche , &cGauche , &tDroit , &cDroit ) ;
            if ( ! verifierJointure ( fichierD , tGauche , cGauche , tDroit , cDroit ) ) {
                cout << "jointure pas correcte" << endl ;
            }
            else {
                fichierR . jointures [ indiceJointure ] . opGauche = tGauche + '.' + cGauche ;
                fichierR . jointures [ indiceJointure ] . opDroit = tDroit + '.' + cDroit ;
                indiceJointure++ ;
            }
        }
        else {
            conditionParseur ( line , &opGauche , &operateur , &opDroit ) ;
            fichierR . conditions [ indiceCondition ] . opGauche = opGauche ;
            fichierR . conditions [ indiceCondition ] . operateur = operateur ;
            fichierR . conditions [ indiceCondition ] . opDroit = opDroit ;
            indiceCondition++ ;
        }
    }
}

string donneToString ( Donnee donnee ) {
    if ( donnee . str == 0 ) {
        return to_string ( donnee . entier ) ;
    }
    else {
        return *( donnee . str ) ;
    }
}

bool chackLineJointure ( Jointure jointure , string *line , string *nomsColonnes , size_t nbTotaleColonnes ) {
    size_t indiceOpGauche = 0 ;
    size_t indiceOpDroit = 0 ;
    for ( size_t i = 0 ; i < nbTotaleColonnes ; i++ ) {
        if ( nomsColonnes [ i ] == jointure . opGauche ) {
            indiceOpGauche = i ;
            break ;
        }
    }
    for ( size_t i = 0 ; i < nbTotaleColonnes ; i++ ) {
        if ( nomsColonnes [ i ] == jointure . opDroit ) {
            indiceOpDroit = i ;
            break ;
        }
    }
    return ( line [ indiceOpGauche ] == line [ indiceOpDroit ] ) ;
}

bool chackLineCondition ( Condition condition , string *line , string *nomsColonnes , size_t nbTotaleColonnes ) {
    size_t indiceOpGauche ;
    size_t indiceOpDroit ;
    Donnee d1 ;
    Donnee d2 ;
    if ( isEntier ( condition . opGauche ) ) {
        d1 . entier = stringToInt ( condition . opGauche ) ;
        d1 . str = 0 ;
    }
    else {
        d1 . str = &condition . opGauche ;
        for ( size_t i = 0 ; i < nbTotaleColonnes ; i++ ) {
            if ( nomsColonnes [ i ] == condition . opGauche ) {
                d1 . entier = i ;
                break ;
            }
        }
    }
    if ( isEntier ( condition . opDroit ) ) {
        d2 . entier = stringToInt ( condition . opDroit ) ;
        d2 . str = 0 ;
    }
    else {
        d2 . str = &condition . opDroit ;
        for ( size_t i = 0 ; i < nbTotaleColonnes ; i++ ) {
            if ( nomsColonnes [ i ] == condition . opDroit ) {
                d2 . entier = i ;
                break ;
            }
        }
    }
    if ( condition . operateur == "=" ) {
        if ( d1 . str == 0 && d2 . str == 0 ) {
            return ( d1 . entier == d2 . entier ) ;
        }
        if ( d2 . str == 0 ) {
            return ( stringToInt ( line [ d1 . entier ] ) == d2 . entier ) ;
        }
        else {
            return ( line [ d1 . entier ] == line [ d2 . entier ] ) ;
        }
    }
    if ( condition . operateur == ">" ) {
        if ( d1 . str == 0 && d2 . str == 0 ) {
            return ( d1 . entier > d2 . entier ) ;
        }
        else if ( d2 . str == 0 ) {
            return ( stringToInt ( line [ d1 . entier ] ) > d2 . entier ) ;
        }
        else {
            return ( line [ d1 . entier ] > line [ d2 . entier ] ) ;
        }
    }
    if ( condition . operateur == "<" ) {
        if ( d1 . str == 0 && d2 . str == 0 ) {
            return ( d1 . entier < d2 . entier ) ;
        }
        if ( d2 . str == 0 ) {
            return ( stringToInt ( line [ d1 . entier ] ) < d2 . entier ) ;
        }
        else {
            return ( line [ d1 . entier ] < line [ d2 . entier ] ) ;
        }
    }
    if ( condition . operateur == ">=" ) {
        if ( d1 . str == 0 && d2 . str == 0 ) {
            return ( d1 . entier >= d2 . entier ) ;
        }
        if ( d2 . str == 0 ) {
            return ( stringToInt ( line [ d1 . entier ] ) >= d2 . entier ) ;
        }
        else {
            return ( line [ d1 . entier ] >= line [ d2 . entier ] ) ;
        }
    }
    if ( condition . operateur == "<=" ) {
        if ( d1 . str == 0 && d2 . str == 0 ) {
            return ( d1 . entier <= d2 . entier ) ;
        }
        if ( d2 . str == 0 ) {
            return ( stringToInt ( line [ d1 . entier ] ) <= d2 . entier ) ;
        }
        else {
            return ( line [ d1 . entier ] <= line [ d2 . entier ] ) ;
        }
    }
}

bool checkLine ( Requete fichierR , string *line , string *nomsColonnes , size_t nbTotaleColonnes ) {
    bool tmp = true ;
    for ( size_t i = 0 ; i < fichierR . nbJointures ; i++ ) {
        if ( ! chackLineJointure ( fichierR . jointures [ i ] , line , nomsColonnes , nbTotaleColonnes ) ) {
            tmp = false ;
        }
    }
    for ( size_t i = 0 ; i < fichierR . nbConditions ; i++ ) {
        if ( ! chackLineCondition ( fichierR . conditions [ i ] , line , nomsColonnes , nbTotaleColonnes ) ) {
            tmp = false ;
        }
    }
    return tmp ;
}

void produitCartesien(string **tabMax,string **source1,size_t s1Lignes,size_t s1Colonnes,string **source2,size_t s2Lignes,size_t s2Colonnes,FichierDonnees fichierD,Requete fichierR,Donnee *donnees,string *strings,string *nomsColonnes,size_t nbTotaleColonnes,size_t *nbLignesChecked) {
    size_t nbLignes = s1Lignes * s2Lignes ;
    size_t nbColonnes = s1Colonnes + s2Colonnes ;
    string *line = new string [ nbTotaleColonnes ] ;
    for ( size_t i = 0 ; i < nbLignes ; i++ ) {
        for ( size_t j = 0 ; j < s1Colonnes ; j++ ) {
            line [ j ] = source1 [ i % s1Lignes ] [ j ] ;
        }
        for ( size_t j = s1Colonnes ; j < nbColonnes ; j++ ) {
            line [ j ] = source2 [ i / s1Lignes ] [ j - s1Colonnes ] ;
        }
        if ( checkLine ( fichierR , line , nomsColonnes , nbTotaleColonnes ) ) {
            ( *nbLignesChecked )++ ;
            for ( size_t j = 0 ; j < s1Colonnes ; j++ ) {
                tabMax [ *nbLignesChecked ] [ j ] = source1 [ i % s1Lignes ] [ j ] ;
            }
            for ( size_t j = s1Colonnes ; j < nbColonnes ; j++ ) {
                tabMax [ *nbLignesChecked ] [ j ] = source2 [ i / s1Lignes ] [ j - s1Colonnes ] ;
            }
        }
    }
    for ( size_t i = 0 ; i < *nbLignesChecked ; i++ ) {
        for ( size_t j = 0 ; j < nbColonnes ; j++ ) {
            bool tmp = false ;
            for ( size_t k = 0 ; k < fichierR . nbColonnes ; k++ ) {
                if ( nomsColonnes [ j ] == fichierR . colonnes [ k ] ) {
                    tmp = true ;
                }
            }
            if ( tmp ) {
                cout << tabMax [ i ] [ j ] << "     " ;
            }
        }
        cout << endl ;
    }
}

void produitMultiple(size_t nbTableaux,string *nomsTableaux,string **tabMax,size_t nbLignes,size_t nbColonnes,FichierDonnees fichierD,Requete fichierR,Donnee *donnees,string *strings,string *nomsColonnes,size_t nbTotaleColonnes,size_t *nbLignesChecked){
    *nbLignesChecked = 0 ;
    if ( nbTableaux == 1 ) {
        return ;
    }
    if ( nbTableaux == 2 ) {
        size_t s1Lignes = getTabParNom ( fichierD , nomsTableaux [ 0 ] ) . nbLignes ;
        size_t s1Colonnes = getTabParNom ( fichierD , nomsTableaux [ 0 ] ) . nbColonnes ;
        string **source1 = new string* [ s1Lignes ] ;
        for ( size_t i = 0 ; i < s1Lignes ; i++ ) {
            source1 [ i ] = new string [ s1Colonnes ] ;
        }
        for ( size_t i = 0 ; i < s1Lignes ; i++ ) {
            for ( size_t j = 0 ; j < s1Colonnes ; j++ ) {
                source1 [ i ] [ j ] = donneToString ( getValeur ( nomsTableaux [ 0 ] , getTabParNom ( fichierD , nomsTableaux [ 0 ] ) . colonnes [ j ] . nom , i , donnees , fichierD ) ) ;
            }
        }
        size_t s2Lignes = getTabParNom ( fichierD , nomsTableaux [ 1 ] ) . nbLignes ;
        size_t s2Colonnes = getTabParNom ( fichierD , nomsTableaux [ 1 ] ) . nbColonnes ;
        string **source2 = new string* [ s2Lignes ] ;
        for ( size_t i = 0 ; i < s2Lignes ; i++ ) {
            source2 [ i ] = new string [ s2Colonnes ] ;
        }
        for ( size_t i = 0 ; i < s2Lignes ; i++ ) {
            for ( size_t j = 0 ; j < s2Colonnes ; j++ ) {
                source2 [ i ] [ j ] = donneToString ( getValeur ( nomsTableaux [ 1 ] , getTabParNom ( fichierD , nomsTableaux [ 1 ] ) . colonnes [ j ] . nom , i , donnees , fichierD ) ) ;
            }
        }
        produitCartesien(tabMax , source1 , s1Lignes , s1Colonnes , source2 , s2Lignes , s2Colonnes , fichierD , fichierR , donnees , strings , nomsColonnes , nbTotaleColonnes , nbLignesChecked ) ;
        return ;
    }
    else {
        size_t s1Lignes = getTabParNom ( fichierD , nomsTableaux [ nbTableaux - 1 ] ) . nbLignes ;
        size_t s1Colonnes = getTabParNom ( fichierD , nomsTableaux [ nbTableaux - 1 ] ) . nbColonnes ;
        string **source1 = new string* [ s1Lignes ] ;
        for ( size_t i = 0 ; i < s1Lignes ; i++ ) {
            source1 [ i ] = new string [ s1Colonnes ] ;
        }
        for ( size_t i = 0 ; i < s1Lignes ; i++ ) {
            for ( size_t j = 0 ; j < s1Colonnes ; j++ ) {
                source1 [ i ] [ j ] = donneToString ( getValeur ( nomsTableaux [ nbTableaux - 1 ] , getTabParNom ( fichierD , nomsTableaux [ nbTableaux - 1 ] ) . colonnes [ j ] . nom , i , donnees , fichierD ) ) ;
            }
        }
        size_t s2Lignes = nbLignes / s1Lignes ;
        size_t s2Colonnes = nbColonnes - s1Colonnes ;
        string **source2 = new string* [ s2Lignes ] ;
        for ( size_t i = 0 ; i < s2Lignes ; i++ ) {
            source2 [ i ] = new string [ s2Colonnes ] ;
        }
        produitMultiple ( nbTableaux - 1 , nomsTableaux , source2 , s2Lignes , s2Colonnes , fichierD , fichierR , donnees , strings , nomsColonnes , nbTotaleColonnes , nbLignesChecked ) ;
        produitCartesien(tabMax , source1 , s1Lignes , s1Colonnes , source2 , s2Lignes , s2Colonnes , fichierD , fichierR , donnees , strings , nomsColonnes , nbTotaleColonnes , nbLignesChecked ) ;
        return ;
    }
}

void dimensionsResultat ( Tableau &resultat , const FichierDonnees &fichierD , const Requete &fichierR , Donnee *donnees , string *strings ) {
    size_t nbLignesChecked = 0 ;
    size_t nbColonnes = 0 ;
    for ( size_t i = 0 ; i < fichierR . nbTableaux ; i++ ) {
        nbColonnes += getTabParNom ( fichierD , fichierR . tableaux [ i ] ) . nbColonnes ;
    }
    size_t nbLignes = 1 ;
    for ( size_t i = 0 ; i < fichierR . nbTableaux ; i++ ) {
        nbLignes *= getTabParNom ( fichierD , fichierR . tableaux [ i ] ) . nbLignes ;
    }
    string **tabMax = new string* [ nbLignes ] ;
    for ( size_t i = 0 ; i < nbLignes ; i++ ) {
        tabMax [ i ] = new string [ nbColonnes ] ;
    }
    string *nomsColonnes = new string [ nbColonnes ] ;
    size_t indice = 0 ;
    for ( size_t i = 0 ; i < fichierR . nbTableaux ; i++ ) {
        for ( size_t j = 0 ; j < getTabParNom ( fichierD , fichierR . tableaux [ i ] ) . nbColonnes ; j++ ) {
            nomsColonnes [ indice ] = fichierR . tableaux [ i ] + '.' + getTabParNom ( fichierD , fichierR . tableaux [ i ] ) . colonnes [ j ] . nom  ;
            indice++ ;
        }
    }
    produitMultiple ( fichierR . nbTableaux , fichierR . tableaux , tabMax , nbLignes , nbColonnes , fichierD , fichierR , donnees , strings , nomsColonnes , nbColonnes , &nbLignesChecked ) ;
    
}

int main () {
    string fichierDonnees = "data.txt" ;
    string fichierRequete = "requete.txt" ;
    ifstream is ;
    FichierDonnees fichierD ;
    Requete fichierR ;
    openFichier ( is , fichierDonnees ) ;
    nbTableaux ( is , fichierD ) ;
    fichierD . tableaux = new Tableau [ fichierD . nbTableaux ] ;
    nomsTableaux ( is , fichierD ) ;
    nbsColonnes ( is , fichierD ) ;
    nbLignes ( is , fichierD ) ;
    for ( size_t i = 0 ; i < fichierD . nbTableaux ; i++ ) {
        fichierD . tableaux [ i ] . colonnes = new Colonne [ fichierD . tableaux [ i ] . nbColonnes ] ;
    }
    nomsColonnes ( is , fichierD ) ;
    typesColonnes ( is , fichierD ) ;
    size_t total = 0 ;
    for ( size_t i = 0 ; i < fichierD . nbTableaux ; i++ ) {
        total += fichierD . tableaux [ i ] . nbLignes * fichierD . tableaux [ i ] . nbColonnes ;
    }
    string* strings = new string [ total ] ;
    Donnee* donnees = new Donnee [ total ] ;
    lectureValeures ( strings , donnees , fichierD , is ) ;
    is . close () ;
    getNbValeursDifferents ( fichierD , donnees , strings ) ;
    openFichier ( is , fichierRequete ) ;
    requeteDimensions ( is , fichierR ) ;
    fichierR . colonnes = new string [ fichierR . nbColonnes ] ;
    fichierR . tableaux = new string [ fichierR . nbTableaux ] ;
    fichierR . jointures = new Jointure [ fichierR . nbJointures ] ;
    fichierR . conditions = new Condition [ fichierR . nbConditions ] ;
    lectureRequete ( is , fichierR , fichierD ) ;
    is . close () ;
    Tableau resultat ;
    dimensionsResultat ( resultat , fichierD , fichierR , donnees , strings ) ;
    
    
    
    
    
    
    
    /*
    cout << "Nombre de tableaux dans le fichier: " << fichierD . nbTableaux << endl ;
    cout << "Les tableaux: " << endl ;
    for ( size_t i = 0 ; i < fichierD . nbTableaux ; i++ ) {
        cout << fichierD . tableaux [ i ] . nom << ':' << fichierD . tableaux [ i ] . nbLignes << 'X' << fichierD . tableaux [ i ] . nbColonnes << endl ;
        cout << "   Colonnes:" << endl ;
        for ( size_t j = 0 ; j < fichierD . tableaux [ i ] . nbColonnes ; j++ ) {
            cout << "   " << fichierD . tableaux [ i ] . colonnes [ j ] . nom << ':' << fichierD . tableaux [ i ] . colonnes [ j ] . nbValeursDifferentes << endl ;
        }
    }
    cout << "Nombre totale de donnees: " << total << endl ;
    cout << "Requete tableaux: " << endl ;
    for ( size_t i = 0 ; i < fichierR . nbTableaux ; i++ ) {
        cout << fichierR . tableaux [ i ] << endl ;
    }
    cout << "Requete colonnes: " << endl ;
    for ( size_t i = 0 ; i < fichierR . nbColonnes ; i++ ) {
        cout << fichierR . colonnes [ i ] << endl ;
    }
    cout << "Requete jointures: " << endl ;
    for ( size_t i = 0 ; i < fichierR . nbJointures ; i++ ) {
        cout << fichierR . jointures [ i ] . opGauche << endl ;
        cout << fichierR . jointures [ i ] . opDroit << endl ;
    }
    cout << "Requete conditions: " << endl ;
    for ( size_t i = 0 ; i < fichierR . nbConditions ; i++ ) {
        cout << fichierR . conditions [ i ] . opGauche << endl ;
        cout << fichierR . conditions [ i ] . operateur << endl ;
        cout << fichierR . conditions [ i ] . opDroit << endl ;
    }
    */
    return 0 ;
}