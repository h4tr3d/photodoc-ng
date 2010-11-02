#!/bin/bash

source project.conf

cd ../..
ver=$project_default_version

case $packer in
  bzip2|bz2)
    packer_bin=bzip2
    packer_opt=-9
    packer_ext=.bz2
  ;;
  gzip|gz)
    packer_bin=gzip
    packer_opt=-9
    packer_ext=.gz
  ;;
  lzma|xz)
    packer_bin=xz
    packer_opt=-9
    packer_ext=.xz
  ;;
  *)
    # In any other case, create tar file only
    packer_bin=cat
    packer_opt=
    packer_ext=
  ;;
esac

#git archive --format=tar --prefix=$project-$ver/ $tag | $packer_bin $packer_opt > ../$project-$ver.tar$packer_ext
cp -a $project-current $project-$ver
rm $project-$ver/*.pro.user
tar cf - $project-$ver | $packer_bin $packer_opt > $project-$ver.tar$packer_ext
