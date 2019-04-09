#!/bin/bash

 #/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
 #
 # NIST-developed software is provided by NIST as a public
 # service. You may use, copy and distribute copies of the software in
 # any medium, provided that you keep intact this entire notice. You
 # may improve, modify and create derivative works of the software or
 # any portion of the software, and you may copy and distribute such
 # modifications or works. Modified works should carry a notice
 # stating that you changed the software and should note the date and
 # nature of any such change. Please explicitly acknowledge the
 # National Institute of Standards and Technology as the source of the
 # software.
 #
 # NIST-developed software is expressly provided "AS IS." NIST MAKES
 # NO WARRANTY OF ANY KIND, EXPRESS, IMPLIED, IN FACT OR ARISING BY
 # OPERATION OF LAW, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 # WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
 # NON-INFRINGEMENT AND DATA ACCURACY. NIST NEITHER REPRESENTS NOR
 # WARRANTS THAT THE OPERATION OF THE SOFTWARE WILL BE UNINTERRUPTED
 # OR ERROR-FREE, OR THAT ANY DEFECTS WILL BE CORRECTED. NIST DOES NOT
 # WARRANT OR MAKE ANY REPRESENTATIONS REGARDING THE USE OF THE
 # SOFTWARE OR THE RESULTS THEREOF, INCLUDING BUT NOT LIMITED TO THE
 # CORRECTNESS, ACCURACY, RELIABILITY, OR USEFULNESS OF THE SOFTWARE.
 #
 # You are solely responsible for determining the appropriateness of
 # using and distributing the software and you assume all risks
 # associated with its use, including but not limited to the risks and
 # costs of program errors, compliance with applicable laws, damage to
 # or loss of data, programs or equipment, and the unavailability or
 # interruption of operation. This software is not intended to be used
 # in any situation where a failure could cause risk of injury or
 # damage to property. The software developed by NIST employees is not
 # subject to copyright protection within the United States.

if [[ -z "${NS3_MODULE_PATH}" ]]; then
  echo "Waf shell not detected; run after './waf shell' command"
  exit
fi

export OVERWRITE
OVERWRITE=0
if [[ "$#" -gt 0 && $1 == "-f" ]];then
OVERWRITE=1
fi

export numCores=8 # Number of cores on your machine (to parallelize)

if [[ ! -d "scratch" ]];then
echo "ERROR: $0 must be copied to ns-3 directory!" 
exit
fi

export STARTRUN=1
export ENDRUN=10 #Number of runs 10
export tx=100 #transmit probability
export minUE=10 #range for UE
export maxUE=100
export minNf=1 #kept at one to simulate HD2
export maxNf=1
export minNt=10 #Nt will be overridden later but in case the user would like to set a specific range these remain
export maxNt=10 
export dataPoints=10000 #number of simulations conducted
export version="data-HD2-MAC-UE-Based" #Version for logging run output
export container="$version-discovery-scenarios"
mkdir -p $container

#creating the folder structure to hold data
cd $container
for ((UE=$minUE; UE<=$maxUE; UE+=10))
do
	c1="UE$UE"
	mkdir -p $c1
	cd $c1
	c2="theta$tx"
	mkdir -p $c2
	cd $c2
	
	minNt=$(($UE/4))
	maxNt=$((4*$UE))
	
	for ((Nt=$minNt; Nt<=$maxNt; Nt++))
	do
		for ((Nf=$minNf; Nf<=$maxNf; Nf++))
		do
			Nr=$(($Nf*$Nt))
			c3="Nr=$Nr,Nt=$Nt"
			mkdir -p $c3
		done
	done
	cd ../..
done
cd ..

./waf

profile=$(./waf --check-profile | tail -1 | cut -d ' ' -f 3)

#function to run one simulation
function run-scenario () {
    run=${1}
    UE=${2}
    Nt=${3}
    Nf=${4}
    c1=${5}
    c2=${6}
    c3=${7}
    newdir="${container}/${c1}/${c3}/${c2}/${version}-${UE}-${Nr}-${Nt}-${run}"
    if [[ -d $newdir && $OVERWRITE == "0" ]];then
        echo "$newdir exist! Use -f option to overwrite."
        return
    fi
    mkdir -p $newdir
    cd $newdir 
    OUTFILE="$log-${version}-${UE}-${Nr}-${Nt}-${run}.txt"
    echo "UEs in simluation = $UE" >> $OUTFILE
    echo -e "-------------------------------\n" >> $OUTFILE
    if [[ $profile == "optimized" ]]; then
        ../../../../../build/src/lte/examples/ns3-dev-wns3-2017-discovery-optimized --RngRun=$run --txProb=$tx --numUe=$UE --Nf=$Nf --Nt=$Nt >> $OUTFILE 2>&1
    elif [[ $profile == "debug" ]]; then
        ../../../../../build/src/lte/examples/ns3-dev-wns3-2017-discovery-debug --RngRun=$run --txProb=$tx --numUe=$UE --Nf=$Nf --Nt=$Nt >> $OUTFILE 2>&1
    else
        echo "Profile: $profile not found"
        exit
    fi
    cd ../..
}

export -f run-scenario

#itterating over all parameter values for simulation
export simcount=0
for ((UE=$minUE; UE<=$maxUE; UE +=10))
do
	c1="UE$UE"
	
	#determine how many runs we need, =ceil(dataPoints/UE)
	top=$(($dataPoints+$UE-1))
	bottom=$(($UE))
	ENDRUN=$(($top/$bottom))
	
	#determine the range of the pool size
	minNt=$(($UE/4))
	maxNt=$((4*$UE))
	
	for ((Nt=$minNt; Nt<=$maxNt; Nt++))
	do
		for ((Nf=$minNf; Nf<=$maxNf; Nf++))
		do
			Nr=$(($Nf*$Nt))
			c2="Nr=$Nr,Nt=$Nt"
			c3="theta$tx"
			
			for ((run=$STARTRUN; run<=$ENDRUN; run++))
			do
				
				echo "RUN: ${version}-${Nf}-${Nt}-${UE}-${run}"
				simcount=$((simcount + 1))
				run-scenario $run $UE $Nt $Nf $c1 $c2 $c3 &
				[[ $((simcount % numCores)) -eq 0 ]] && wait # parallelize up to numCores jobs
				
			done
		done
    done
done

wait
echo "all simulations complete"

