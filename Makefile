# Copyright 2021 Bruno VERCHÈRE
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

CC          := gcc
CCFLAGS     :=-Wall -O2

SOURCE      := treeBuilder.c
BUILD       := treeBuilder

MKDIR       := mkdir -p
PDFLATEX    := pdflatex
PDF         := tree.pdf
TEMPDIR     := tempdir
TEXFILE     := tree.tex

DOXYGEN     := doxygen
DOCSSRC     := docs/docs
DOCSBUILD   := docs/site

all: $(BUILD) $(TEMPDIR) $(TEXFILE) $(PDF) clean-tex

clean: clean-build clean-tex

clean-build:
	rm -f $(BUILD)

clean-tex:
	rm -f $(TEXFILE)
	rm -rf $(TEMPDIR)

clean-logs:
	rm -f ./logs/*.log

$(BUILD):
	$(CC) -o $@ $(CCFLAGS) $(SOURCE)

$(TEXFILE): $(BUILD)
	./$^

$(TEMPDIR): $(TEXFILE)
	$(MKDIR) $@
	cp $^ $@
	rm $^

$(PDF): $(TEXFILE) $(TEMPDIR)
	$(PDFLATEX) -output-directory $(TEMPDIR) $< > /dev/null
	cp $(TEMPDIR)/$@ .

docs:
	$(DOXYGEN)


.PHONY: all clean docs