import struct
import os

dataFormat = "72s72s72s72s2s8s2s"
dataStruct = struct.Struct(dataFormat)

os.makedirs("data", exist_ok=True)

exemplos = [
    ("Rua A", "Bairro A", "Cidade A", "Estado A", "SP", "01001000", "SP"),
    ("Rua B", "Bairro B", "Cidade B", "Estado B", "RJ", "22041001", "RJ"),
    ("Rua C", "Bairro C", "Cidade C", "Estado C", "MG", "30140071", "MG"),
    ("Rua D", "Bairro D", "Cidade D", "Estado D", "SP", "01001000", "SP"),  # (colisão)
    ("Rua E", "Bairro E", "Cidade E", "Estado E", "BA", "40010000", "BA"),
]

with open("data/cep.dat", "wb") as f:
    for rua, bairro, cidade, estado, sigla, cep, uf in exemplos:
        packed = dataStruct.pack(
            rua.encode().ljust(72, b"\0"),
            bairro.encode().ljust(72, b"\0"),
            cidade.encode().ljust(72, b"\0"),
            estado.encode().ljust(72, b"\0"),
            sigla.encode().ljust(2, b"\0"),
            cep.encode().ljust(8, b"\0"),
            uf.encode().ljust(2, b"\0"),
        )
        f.write(packed)

print("Arquivo data/cep.dat gerado com sucesso.")
