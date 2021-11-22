DROP DATABASE IF EXISTS M5_JUEGO;
CREATE DATABASE M5_JUEGO;

USE M5_JUEGO;

CREATE TABLE Jugador (
    NOMBRE VARCHAR(60),
    CONTRASEÑA VARCHAR(20),
    PRIMARY KEY (NOMBRE)
);

CREATE TABLE Partida (
    ID INTEGER,
    FyH VARCHAR(100),
    UBICACION VARCHAR(100),
    DURACION INTEGER,
    GANADOR VARCHAR(20),
    PRIMARY KEY (ID)
);

CREATE TABLE Puente (
    IDp INTEGER,
    IDj VARCHAR (60),
    PUNTUACION INTEGER,
    FOREIGN KEY (IDp) references Partida(ID),
    FOREIGN KEY (IDj) references Jugador(NOMBRE)
);

INSERT INTO Jugador VALUES ('Leo', 2411);
INSERT INTO Jugador VALUES ('Abel', 1206);
INSERT INTO Jugador VALUES ('Marc', 2306);
INSERT INTO Partida VALUES ('1','6 Octubre 12:30','Girona', 5, 'Leo');
INSERT INTO Partida VALUES ('2','7 Octubre 12:30','Lleida', 5, 'Marc');
INSERT INTO Partida VALUES ('3','8 Octubre 12:30','Sitges', 5, 'Abel');
INSERT INTO Partida VALUES ('4','9 Octubre 12:30','Andorra', 5, 'Abel');
INSERT INTO Puente VALUES ('1','Leo',100);
INSERT INTO Puente VALUES ('2','Leo',70);
INSERT INTO Puente VALUES ('3','Leo',60);
INSERT INTO Puente VALUES ('1','Abel',70);
INSERT INTO Puente VALUES ('3','Abel',100);
INSERT INTO Puente VALUES ('4','Abel',100);
INSERT INTO Puente VALUES ('2','Marc',90);
INSERT INTO Puente VALUES ('4','Marc',80);



