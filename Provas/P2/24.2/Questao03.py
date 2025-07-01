import struct
import hashlib
import os

hashSize = 900001
fileName = "data/cep.dat"
indexName = "data/cep-hash.dat"
dataFormat = "72s72s72s72s2s8s2s"
indexFormat = "8sLL"
keyColumnIndex = 5

def h(key):
  return int(hashlib.sha1(key).hexdigest(), 16) % hashSize

dataStruct = struct.Struct(dataFormat)
indexStruct = struct.Struct(indexFormat)

fi = open(indexName, "rb")
f = open(fileName, "rb")

cepProcurado = b"22071020"
p = h(cepProcurado)
offset = p * indexStruct.size
i = 1

while True:
  fi.seek(offset, os.SEEK_SET)
  indexRecord = indexStruct.unpack(fi.read(indexStruct.size))

  if indexRecord[0] == cepProcurado:
    f.seek(indexRecord[1] * dataStruct.size, os.SEEK_SET)
    record = dataStruct.unpack(f.read(dataStruct.size))
    print(record)
    print(i)
    break

  offset = indexRecord[2]
  if offset == 0:
    break
  i += 1

def maior_colisao():
  maiorQtd = -1
  posMaior = -1

  for i in range(hashSize):
    qtdAtual = 0
    fi.seek(i * indexStruct.size, os.SEEK_SET)
    data = fi.read(indexStruct.size)
    indexRecord = indexStruct.unpack(data)

    while indexRecord[2]:
      qtdAtual += 1
      fi.seek(indexRecord[2], os.SEEK_SET)
      indexRecord = indexStruct.unpack(fi.read(indexStruct.size))

    if qtdAtual > maiorQtd:
      maiorQtd = qtdAtual
      posMaior = i

  return maiorQtd

print("Maior número de colisões:", maior_colisao())

fi.close()
f.close()