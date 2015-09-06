#!/bin/bash

set -e

cd ${0%/*}

pwd

./Papilio_Programmer.sh $1 $0\..
