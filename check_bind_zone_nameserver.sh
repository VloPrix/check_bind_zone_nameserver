#!/bin/bash

#	Nagios Check for checking if the Zones of the local bind server resolves to the correct NS
#
#	Author: Luke Haase
#	Date: 23.04.2024
#	Version: 1.0

################## CONFIGURATION ############################################

# Directory for the .hosts Files
ZONE_DIR=/var/lib/bind/*
# https://www.reddit.com/r/linuxadmin/comments/vsqxvz/script_to_check_if_you_are_still_authoritative/
# Change the domain example.com to an own domain with correct NS Records
CORRECT_DOMAIN_DNS_MD5SUM=$(dig @1.1.1.1 +short -t NS example.com | sort | md5sum)
# Exclusion list to exclude certain files from the check
EXCLUDE=(".key" ".private" "DEADJOE" "md5sum" ".bak" "dsset-" "db-")
# Warning count wrong NS Records
WARNING=10
# Critical count wrong NS Records
CRITICAL=20

#############################################################################

ERROR_DOMAIN_COUNT=0
ERROR_DOMAIN_LIST=""

isExcluded () {
  for i in "${EXCLUDE[@]}"
  do
    if [[ $1 == *"$i"* ]]
    then
      return 0
    fi
  done
  return 1
}



for file in $ZONE_DIR
do
  DOMAIN="${file##*/}"
  DOMAIN=${DOMAIN/".hosts"/}
  MD5SUM=$(dig @1.1.1.1 +short -t NS $DOMAIN | sort | md5sum)
  isExcluded $DOMAIN
  ISEXCLUDED=$?
  if [ $ISEXCLUDED -eq 0 ]; then
    continue
  fi
  if [[ "$MD5SUM" != "$CORRECT_DOMAIN_DNS_MD5SUM" ]]; then
    ERROR_DOMAIN_COUNT=$((++ERROR_DOMAIN_COUNT))
    ERROR_DOMAIN_LIST="$ERROR_DOMAIN_LIST$DOMAIN, "
  fi
done

if [[ $ERROR_DOMAIN_COUNT -ge $CRITICAL ]]; then
  logger "checkBindZones CRITICAL:$ERROR_DOMAIN_COUNT Domains with incorrect NS: $ERROR_DOMAIN_LIST"
  echo "checkBindZones CRITICAL: $ERROR_DOMAIN_COUNT Domains with incorrect NS present: $ERROR_DOMAIN_LIST"
  exit 2
elif [[ $ERROR_DOMAIN_COUNT -ge $WARNING ]]; then
  logger "checkBindZones WARNING:$ERROR_DOMAIN_COUNT Domains with incorrect NS: $ERROR_DOMAIN_LIST"
  echo "checkBindZones WARNING: $ERROR_DOMAIN_COUNT Domains with incorrect NS present: $ERROR_DOMAIN_LIST"
  exit 1
else
  echo "checkBindZones OK: $ERROR_DOMAIN_COUNT Domains with incorrect NS present!"
  exit 0
fi