#!/bin/bash

#key='K_UKQuz3tg!9)T%4lEkrgXD9t2odN*Zd8AND_$Bj)Iooiy(J3y&esAi'
#data='hwOUtuXw_H^1AxfP6t4jR*TPMP_a^(LdLv$a68x!uIR_6XV8mU&aS66ABVAM_h6BSZBE3%vcDjiEc4HoDEErFqR7!*P2uAy$v&Nd_9ftnn%p&BUQbe8HuFJCSez$V%c&_Pa1Tvq8JksV2#H4*vGH&lfOGVXhPZdFutB%PR*u*pl^iT%v%A(_LeUnZyuutO^OdwYSkVix_usojmJtMIzu3)HHN(*83iR6EvU5m3issAHBN&UvU)6CpJf3Xs6FACGb$gbvfjOx1b^Jn6ae67GqL3(oaa0RCv9MS08X)Bq^SlEc*Eg(q3o*KCB2Cqyb3CNQrQtG(ZB7!69eqkcW$GUT9bAHI%Iqwr7O8vqbaS9hD#l0tDMm0Cb%TCRhMfN0wPTz_0BhXFohNQm7)Yofwqp&S7dkhRpTm9(t%zAR55%yQhAQbP!y6guT#N)pkp9Nt77DmCrmml14IhP1#WeX3Ow_hvfI1$ra^dTxV_KdcGc14RGVJ1yru_xSCSwy2NOXQoaHe1KwnNd&AZ9_Gmx&t!O!J_prD5kpok#shD6K&9WM4KCp$&mm8&twdI4WI$#Wua^BD1hP54i4u1)n#BPZx9sAMbdbllXVl9s65zA5iYOSV46NkQtXULun3Noj%h5vmcgmBX*KB2is1k7_&f4R7d557o%1vYcX2i_ibd9D5n!KNXQJ(Pga0_6Ve^CQTEtQ3j^5C4Dmmu#ZoYFmuLnjS(5W$CI373z6hu6Kto#WYbR#^cfp5IOCGPeP'
key=$1
data=$2

test/bin/put $key $data
orig=`echo $data | wc -c`
result=`test/bin/get_by_key $key | wc -c`

echo "data: "$orig
echo "returned: "$result

if [ $orig -eq $result ]
then
	echo "Success!"
else
	echo "Fail!"
fi
