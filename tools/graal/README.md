# Generating reflection json file

1. You need to package your java program in a JAR files.

2. Check the native-image-configure tool by command:
```bash
native-image-configure --help
```
3. Execute the command below (we assume that your JAR file named demo.jar):
```bash
java -agentlib:native-image-agent=trace-output=./trace-file.json -jar demo.jar
```
4. Generate the reflection config json file.
```bash
native-image-configure process-trace --output-dir=./graal/ ./trace-file.json
```
5. Then, you will find `reflection_config.json` in the output directory: `./graal/`.

6. More detail: [CONFIGURE.md](https://github.com/oracle/graal/blob/master/substratevm/CONFIGURE.md), [REFLECTION.md](https://github.com/oracle/graal/blob/master/substratevm/REFLECTION.md).