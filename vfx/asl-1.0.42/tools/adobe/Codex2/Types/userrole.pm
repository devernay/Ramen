# =========================================================================
# Formatting: Expand tabs, 4 spaces per indent level.
# =========================================================================

=pod

=head1 NAME

Adobe::Codex2::Types::userrole - Define Codex type data.

=head1 DESCRIPTION

See POD from Adobe::Codex2.pm for details on using the Codex modules.

=head2 XML

  <xs:element name="userrole">
    <xs:complexType>
      <xs:attribute name="userid" type="xs:string" use="required"/>
      <xs:attribute name="productname" type="xs:string" use="required"/>
      <xs:attribute name="rolename" type="xs:string" use="required"/>
      <xs:attribute name="id" type="xs:integer" use="required"/>      
    </xs:complexType>
  </xs:element>

=head1 SUBROUTINES/METHODS

=cut

# -------------------------------------------------------------------------
# Module setup.
# -------------------------------------------------------------------------
package Adobe::Codex2::Types::userrole;

require 5.8.0;
use strict;
use warnings;

use base qw(Exporter);
our $VERSION     = "0.2.0";
our @ISA         = qw();
our @EXPORT      = qw();
our @EXPORT_OK   = qw(&Data);
our %EXPORT_TAGS = ();

# Get Perl modules.
# None.

# Get Codex modules.
# None.

# -------------------------------------------------------------------------
# Data
# -------------------------------------------------------------------------
sub Data
{
    # Get arguments.
    my $Data = shift;

    # Check arguments.
    # TODO
    
    # Check data.
    # TODO

    # Storage for results.
    my %userrole;

    # Process data.
    $userrole{'id'}          = $Data->{'id'};
    $userrole{'productname'} = $Data->{'productname'};
    $userrole{'rolename'}    = $Data->{'rolename'};
    $userrole{'userid'}      = $Data->{'userid'};

    # Return results.
    return %userrole;
}

1;
__END__

=pod

=head1 AUTHOR

    Dave Foglesong (fogleson@adobe.com)

=head1 COPYRIGHT

    Copyright 2008 Adobe Systems Incorporated. All rights reserved.

=cut

