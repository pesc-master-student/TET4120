webtalk_init -webtalk_dir C:\\Programming\\PESC_SDK_CTRL\\webtalk
webtalk_register_client -client project
webtalk_add_data -client project -key date_generated -value "2022-08-30 19:52:21" -context "software_version_and_target_device"
webtalk_add_data -client project -key product_version -value "SDK v2019.1" -context "software_version_and_target_device"
webtalk_add_data -client project -key build_version -value "2019.1" -context "software_version_and_target_device"
webtalk_add_data -client project -key os_platform -value "amd64" -context "software_version_and_target_device"
webtalk_add_data -client project -key registration_id -value "209875111_209874231_210634734_141" -context "software_version_and_target_device"
webtalk_add_data -client project -key tool_flow -value "SDK" -context "software_version_and_target_device"
webtalk_add_data -client project -key beta -value "false" -context "software_version_and_target_device"
webtalk_add_data -client project -key route_design -value "NA" -context "software_version_and_target_device"
webtalk_add_data -client project -key target_family -value "NA" -context "software_version_and_target_device"
webtalk_add_data -client project -key target_device -value "NA" -context "software_version_and_target_device"
webtalk_add_data -client project -key target_package -value "NA" -context "software_version_and_target_device"
webtalk_add_data -client project -key target_speed -value "NA" -context "software_version_and_target_device"
webtalk_add_data -client project -key random_id -value "anpaeujld063hd62pdt7kqvp4k" -context "software_version_and_target_device"
webtalk_add_data -client project -key project_id -value "2019.1_49" -context "software_version_and_target_device"
webtalk_add_data -client project -key project_iteration -value "49" -context "software_version_and_target_device"
webtalk_add_data -client project -key os_name -value "Microsoft Windows 8 or later , 64-bit" -context "user_environment"
webtalk_add_data -client project -key os_release -value "major release  (build 9200)" -context "user_environment"
webtalk_add_data -client project -key cpu_name -value "Intel(R) Core(TM) i7-7600U CPU @ 2.80GHz" -context "user_environment"
webtalk_add_data -client project -key cpu_speed -value "2904 MHz" -context "user_environment"
webtalk_add_data -client project -key total_processors -value "1" -context "user_environment"
webtalk_add_data -client project -key system_ram -value "17.059 GB" -context "user_environment"
webtalk_register_client -client sdk
webtalk_add_data -client sdk -key uid -value "1661880599589" -context "sdk\\\\application/1661880599589"
webtalk_add_data -client sdk -key hwid -value "1622721849312" -context "sdk\\\\application/1661880599589"
webtalk_add_data -client sdk -key bspid -value "1639760283838" -context "sdk\\\\application/1661880599589"
webtalk_add_data -client sdk -key newbsp -value "false" -context "sdk\\\\application/1661880599589"
webtalk_add_data -client sdk -key os -value "standalone" -context "sdk\\\\application/1661880599589"
webtalk_add_data -client sdk -key apptemplate -value "empty_application" -context "sdk\\\\application/1661880599589"
webtalk_add_data -client sdk -key RecordType -value "APPCreation" -context "sdk\\\\application/1661880599589"
webtalk_add_data -client sdk -key LangUsed -value "C" -context "sdk\\\\application/1661880599589"
webtalk_add_data -client sdk -key Procused -value "ps7_cortexa9" -context "sdk\\\\application/1661880599589"
webtalk_add_data -client sdk -key uid -value "1661880644576" -context "sdk\\\\application/1661880644576"
webtalk_add_data -client sdk -key hwid -value "1622721849312" -context "sdk\\\\application/1661880644576"
webtalk_add_data -client sdk -key bspid -value "1639760283838" -context "sdk\\\\application/1661880644576"
webtalk_add_data -client sdk -key newbsp -value "false" -context "sdk\\\\application/1661880644576"
webtalk_add_data -client sdk -key os -value "standalone" -context "sdk\\\\application/1661880644576"
webtalk_add_data -client sdk -key apptemplate -value "empty_application" -context "sdk\\\\application/1661880644576"
webtalk_add_data -client sdk -key RecordType -value "APPCreation" -context "sdk\\\\application/1661880644576"
webtalk_add_data -client sdk -key LangUsed -value "CPP" -context "sdk\\\\application/1661880644576"
webtalk_add_data -client sdk -key Procused -value "ps7_cortexa9" -context "sdk\\\\application/1661880644576"
webtalk_add_data -client sdk -key projSize -value "124.94140625" -context "sdk\\\\application/1661880644576"
webtalk_add_data -client sdk -key uid -value "NA" -context "sdk\\\\bsp/1661881941188"
webtalk_add_data -client sdk -key RecordType -value "ToolUsage" -context "sdk\\\\bsp/1661881941188"
webtalk_add_data -client sdk -key BootgenCount -value "0" -context "sdk\\\\bsp/1661881941188"
webtalk_add_data -client sdk -key DebugCount -value "0" -context "sdk\\\\bsp/1661881941188"
webtalk_add_data -client sdk -key PerfCount -value "0" -context "sdk\\\\bsp/1661881941188"
webtalk_add_data -client sdk -key FlashCount -value "0" -context "sdk\\\\bsp/1661881941188"
webtalk_add_data -client sdk -key CrossTriggCount -value "0" -context "sdk\\\\bsp/1661881941188"
webtalk_add_data -client sdk -key QemuDebugCount -value "0" -context "sdk\\\\bsp/1661881941188"
webtalk_transmit -clientid 1198925899 -regid "209875111_209874231_210634734_141" -xml C:\\Programming\\PESC_SDK_CTRL\\webtalk\\usage_statistics_ext_sdk.xml -html C:\\Programming\\PESC_SDK_CTRL\\webtalk\\usage_statistics_ext_sdk.html -wdm C:\\Programming\\PESC_SDK_CTRL\\webtalk\\sdk_webtalk.wdm -intro "<H3>SDK Usage Report</H3><BR>"
webtalk_terminate
