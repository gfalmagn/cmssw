from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.section_('General')
config.General.requestName = 'DoubleMu_Run2015E-PromptReco-v1_Run_262081_262328_ONIATREE_20151223'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'hioniaanalyzer_ppPrompt_cfg.py'
config.JobType.outputFiles = ['OniaTree.root']

config.section_('Data')
config.Data.inputDataset ='/DoubleMu/anstahll-DoubleMu_Run2015E-PromptReco-v1_Run_262081_262328_ONIASKIM-e7b651a955cf125e7d99053cfa95fb78/USER'
config.Data.inputDBS = 'phys03'
config.Data.unitsPerJob = 100
config.Data.splitting = 'LumiBased'
config.Data.runRange = '262081-262328'
config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/5TeV/DCSONLY/json_DCSONLY.txt'
config.Data.outLFNDirBase = '/store/user/%s/PromptReco/%s' % (getUsernameFromSiteDB(), config.General.requestName)
config.Data.publication = False

config.section_('Site')
config.Site.whitelist = ["T2_FR_GRIF_LLR"]
config.Site.storageSite = 'T2_FR_GRIF_LLR'

# If your site is blacklisted by crab, use:
# config.Data.ignoreLocality = True
# config.Site.whitelist = ["T2_FR*"]
