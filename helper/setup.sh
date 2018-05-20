# Setup instruction for benchmarks on digitalocean droplet.
# Ubuntu 18.04

apt-get update
echo LC_ALL="en_US.UTF-8" >> /etc/environment
reboot

apt-get install build-essential
apt-get install postgresql postgresql-contrib postgresql-server-dev-10

# create root superuser
sudo -u postgres createuser --interactive
createdb root

ssh-keygen -t rsa
git clone git@github.com:eschmar/postgres-bit-count.git
cd postgres-bit-count

make install
make installcheck

psql
\timing
CREATE EXTENSION bit_count;
\q

sh helper/experiment.sh
