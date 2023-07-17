run=`bjobs | grep RUN | wc -l`
pend=`bjobs | grep PEND | wc -l`
tot=$(($run + $pend))

echo "Running jobs $run"
echo "Pending jobs $pend"
echo "Total $tot"
