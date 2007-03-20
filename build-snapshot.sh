VERSION=0.33
DATE=`date +%Y%m%d`
RELEASE=2.8
TARBALL=olpc-artwork-$VERSION-$RELEASE.${DATE}git.tar.gz

rm olpc-artwork-$VERSION.tar.gz

make distcheck

mv olpc-artwork-$VERSION.tar.gz $TARBALL
scp $TARBALL mpg@devserv.devel.redhat.com:~
rm $TARBALL
