 #!/bin/sh

#create enitlements.plist

clear; clear; 



# APP_NAME='ToyThelemacus'
APP_NAME='Thelemacus'

#to show the keys do `security find-identity -p basic -v`

#new mac
USER_NAME='michelecastellana'
DEVELOPER_ID_APPLICATION='8AB2C11D2047F2B295E096E25ABE327C09929797'
DEVELOPER_ID_INSTALLER='FF42D55EDD167D5C6ED617A193A843C57CDB0E8B'

#old mac
# USER_NAME='macbookpro'
# DEVELOPER_ID_APPLICATION='A681D126EC521DC17837FA01B5EA0B51CD9C961E'
# DEVELOPER_ID_INSTALLER='83FCCF7AF76288BF5120A6AF2C9E5C0233AD999A'

APP_IN_PATH='/Users/'$USER_NAME'/Desktop'
PKG_OUT_PATH='/Users/'$USER_NAME'/Desktop/install'
TEMP_PATH='/Users/'$USER_NAME'/Desktop/temp'

rm -rf $PKG_OUT_PATH $TEMP_PATH
mkdir -p $PKG_OUT_PATH $TEMP_PATH'/'$APP_NAME'.app'

echo "\n\nSigning the code and its libraries ... "
codesign --deep --force --options=runtime --entitlements ./entitlements.plist --sign $DEVELOPER_ID_APPLICATION --timestamp $APP_IN_PATH'/'$APP_NAME'.app'/Contents/Resources/Libraries/*
codesign --deep --force --options=runtime --entitlements ./entitlements.plist --sign $DEVELOPER_ID_APPLICATION --timestamp $APP_IN_PATH'/'$APP_NAME'.app'
echo " ... done"

ditto $APP_IN_PATH'/'$APP_NAME'.app' $TEMP_PATH'/'$APP_NAME'.app'
 
echo "\n\nBuilding the .pkg installer ... "
productbuild --identifier "com.your.pkgname.pkg" --sign $DEVELOPER_ID_INSTALLER --timestamp --component $TEMP_PATH'/'$APP_NAME'.app'  /Applications $PKG_OUT_PATH'/'$APP_NAME'.pkg'
echo " .... done."

echo "\n\nChecking the signature ... "
pkgutil --check-signature $PKG_OUT_PATH'/'$APP_NAME'.pkg'
        # Package "Thelemacus.pkg":
        # Status: signed by a developer certificate issued by Apple for distribution
        # Signed with a trusted timestamp on: 2024-12-10 16:39:55 +0000
        # Certificate Chain:
        #     1. Developer ID Installer: MICHELE CASTELLANA (V3N3VGR839)
        #     Expires: 2027-02-01 22:12:15 +0000
        #     SHA256 Fingerprint:
        #         B2 79 A4 1C 33 86 10 83 88 14 45 B4 81 06 5F CA DB 6D 17 CE 24 3E 
        #         17 FC 39 86 53 A6 CB 6A BB C0
        #     ------------------------------------------------------------------------
        #     2. Developer ID Certification Authority
        #     Expires: 2027-02-01 22:12:15 +0000
        #     SHA256 Fingerprint:
        #         7A FC 9D 01 A6 2F 03 A2 DE 96 37 93 6D 4A FE 68 09 0D 2D E1 8D 03 
        #         F2 9C 88 CF B0 B1 BA 63 58 7F
        #     ------------------------------------------------------------------------
        #     3. Apple Root CA
        #     Expires: 2035-02-09 21:40:36 +0000
        #     SHA256 Fingerprint:
        #         B0 B1 73 0E CB C7 FF 45 05 14 2C 49 F1 29 5E 6E DA 6B CA ED 7E 2C 
        #         68 C5 BE 91 B5 A1 10 01 F0 24
echo "... done."


echo "\n\nSubmitting the app for notarization ... "

notarytool_response=$( xcrun notarytool submit --apple-id "michele.castellana@gmail.com" --team-id "V3N3VGR839" --password "vwfw-otqv-pcir-nmfw" $PKG_OUT_PATH'/'$APP_NAME'.pkg' --wait )

echo $notarytool_response
submission_id=$(echo "$notarytool_response" | awk '/id: / { print $2;exit; }')
echo "... done\nSubmission ID: $submission_id"

echo "\n\nChecking the status of the submission ... "
xcrun notarytool info --apple-id "michele.castellana@gmail.com" --team-id "V3N3VGR839" --password "vwfw-otqv-pcir-nmfw" $submission_id

        # Successfully received submission info
        # createdDate: 2024-12-10T16:41:07.037Z
        # id: be289d0d-f420-49ce-8096-c0df1e7f3eb3
        # name: Thelemacus.pkg
        # status: Accepted
echo "... done."


echo "\nPrinting the log of the submission ... "
xcrun notarytool log --apple-id "michele.castellana@gmail.com" --team-id "V3N3VGR839" --password "vwfw-otqv-pcir-nmfw" $submission_id

            # {
            # "logFormatVersion": 1,
            # "jobId": "be289d0d-f420-49ce-8096-c0df1e7f3eb3",
            # "status": "Accepted",
            # "statusSummary": "Ready for distribution",
            # "statusCode": 0,
            # "archiveFilename": "Thelemacus.pkg",
            # "uploadDate": "2024-12-10T16:41:08.718Z",
            # "sha256": "6eed95b5169900c5a98ce042bbc394f1d440319db4c707ec225071c7e870a6e2",
            # "ticketContents": [
            #     {
            #     "path": "Thelemacus.pkg",
            #     "digestAlgorithm": "SHA-1",
            #     "cdhash": "5ab5913feb5fb82dd7c9eb5a96d73f3376c873ad"
            #     },
            #     {
            #     "path": "Thelemacus.pkg'/'$APP_NAME'.pkg' Contents/Payload/Applications",
            #     "digestAlgorithm": "SHA-256",
            #     "cdhash": "75655762cf598f92a7526c46950f98055455da88",
            #     "arch": "arm64"
            #     }
            # ],
            # "issues": null
            # }
echo "... done."

echo "\n\nStapling the .pkg file ..."
xcrun stapler staple $PKG_OUT_PATH'/'$APP_NAME'.pkg'
echo "... done"

            # Processing: /Users/michelecastellana/Documents/thelemacus'/'$APP_NAME'.pkg'
            # Processing: /Users/michelecastellana/Documents/thelemacus'/'$APP_NAME'.pkg'
            # The staple and validate action worked!



# echo "\n\nPrinting my submission history ... "
# xcrun notarytool history  --apple-id "michele.castellana@gmail.com" --team-id "V3N3VGR839" --password "vwfw-otqv-pcir-nmfw"
# echo " ... done"

rm -rf $TEMP_PATH
